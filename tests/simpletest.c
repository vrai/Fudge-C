/**
 * Copyright (C) 2009 - 2009, Vrai Stacey.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 *     
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "simpletest.h"
#include <stdlib.h>
#ifdef FUDGE_HAVE_MATH_H
#include <math.h>
#endif /* ifdef FUDGE_HAVE_MATH_H */
#ifdef FUDGE_HAVE_UNISTD_H
#include <unistd.h>
#endif /* ifdef FUDGE_HAVE_UNISTD_H */
#ifdef FUDGE_HAVE_TIME_H
#include <time.h>
#endif /* ifdef FUDGE_HAVE_TIME_H */
#ifdef FUDGE_HAVE_STDARG_H
#include <stdarg.h>
#endif /* ifdef FUDGE_HAVE_STDARG_H */
#include "snprintf.h"
#include "ansi_compat.h"

typedef struct TestFunctorList
{
    int ( *functor ) ( );
    const char * name;
    struct TestFunctorList * next;
} TestFunctorList;

static const char * g_suiteName = 0;
static const char * g_testName = 0;
static int g_fileDescriptor = 0;
static fpos_t g_filePosition;
static TestFunctorList * g_testlist = 0;
jmp_buf g_simpleTest_jmpBuffer;

void SimpleTest_log ( const char * format, ... )
{
    char timeString [ 64 ];
    struct tm utcTime;
    va_list args;

    /* Construct the timestamp and output the log line prefix */
    time_t now = time ( 0 );
    gmtime_r ( &now, &utcTime );
    strftime ( timeString, sizeof ( timeString ), "%Y-%m-%d %H:%M:%S", &utcTime );
    fprintf ( stdout, "--- %s [%s", timeString, g_suiteName );
    if ( g_testName )
        fprintf ( stdout, "/%s", g_testName );
    fprintf ( stdout, "] " );

    /* Output the text and arguments, then terminate the line */
    va_start ( args, format );
    vfprintf ( stdout, format, args );
    va_end ( args );
    fprintf ( stdout, "\n" );
}

void SimpleTest_logHostAndPid ( )
{
    char hostname [ 256 ];
    if ( gethostname ( hostname, sizeof ( hostname ) ) )
        snprintf ( hostname, sizeof ( hostname ), "UNKNOWN-HOST" );

    SimpleTest_log ( "Running on host \"%s\" with PID %d", hostname,
#ifdef FUDGE_HAS_GETPID
        getpid ( )
#else /* ifdef FUDGE_HAS_GETPID */
        -1
#endif /* ifdef FUDGE_HAS_GETPID */
    );
}

void SimpleTest_initialiseSuite ( const char * name )
{
    char filename [ 256 ];

    /* As the test framework uses global variables it isn't thread-safe at the suite level, make sure that initialise
       isn't called more than once. */
    if ( g_suiteName || g_fileDescriptor )
    {
        fprintf ( stderr, "Must not call SimpleTest_initialiseSuite more than once!\n"
                          "Ensure that it is only called via DEFINE_TEST_SUITE.\n" );
        exit ( 1 );
    }

    g_suiteName = name;

    /* Redirect stdout to a log file */
#ifdef FUDGE_HAS_DUP
    fgetpos ( stdout, &g_filePosition );
    g_fileDescriptor = dup ( fileno ( stdout ) );
#else /* ifdef FUDGE_HAS_DUP */
    g_fileDescriptor = -1;
    g_filePosition = -1;
#endif /* ifdef FUDGE_HAS_DUP */
    snprintf ( filename, sizeof ( filename ), "test_%s.log", name );
    if ( ! freopen ( filename, "w", stdout ) )
    {
        fprintf ( stderr, "Failed to reopen stdout stream with filename \"%s\".\n", filename );
        exit ( 1 );
    }

    /* Log output header */
    SimpleTest_log ( "Log file for test suite \"%s\"", g_suiteName );
    SimpleTest_logHostAndPid ( );
}

int SimpleTest_runSuite ( )
{
    TestFunctorList * testlist;
    int testresult;

    fprintf ( stderr, "Running test suite \"%s\" with %d tests:\n", g_suiteName, SimpleTest_countTests ( ) );
    for ( testlist = g_testlist; testlist; testlist = testlist->next )
    {
        fprintf ( stderr, "    %s ... ", testlist->name );
        fflush ( stderr );

        g_testName = testlist->name;
        SimpleTest_log ( "Running test \"%s\"", g_testName );
        testresult = testlist->functor ( );
        SimpleTest_log ( "Finished test \"%s\" with result code %d", g_testName, testresult );
        g_testName = 0;

        if ( testresult )
            return testresult;
        else
            fprintf ( stderr, "OK\n" );
    }
    return 0;
}

void SimpleTest_cleanupSuite ( )
{
    /* Close the log file and restore stdout */
    fflush ( stdout );
#ifdef FUDGE_HAS_DUP
    dup2 ( g_fileDescriptor, fileno ( stdout ) );
    close ( g_fileDescriptor );
    clearerr ( stdout );
    fsetpos ( stdout, &g_filePosition );
#else /* ifdef FUDGE_HAS_DUP */
    // Naughty assumption that we were writing to the console originally
    freopen ("CON", "w", stdout);
#endif /* ifdef FUDGE_HAS_DUP */

    /* Destroy the registed tests (makes it easier to spot true memory leaks in valgrind) */
    while ( g_testlist )
    {
        TestFunctorList * temp = g_testlist;
        g_testlist = g_testlist->next;
        free ( temp );
    }
}

void SimpleTest_registerTest ( const char * name, int ( *test ) ( ) )
{
    TestFunctorList * list = ( TestFunctorList * ) malloc ( sizeof ( TestFunctorList ) );
    if ( ! list )
    {
        fprintf ( stderr, "Ran out of memory while registering test \"%s\" in suite \"%s\n", name, g_suiteName );
        exit ( 1 );
    }
    list->functor = test;
    list->name = name;
    list->next = 0;

    if ( g_testlist )
    {
        TestFunctorList * testlist = g_testlist;
        while ( testlist->next )
            testlist = testlist->next;
        testlist->next = list;
    }
    else
        g_testlist = list;
}

int SimpleTest_countTests ( )
{
    int count = 0;
    TestFunctorList * testlist = g_testlist;

    while ( testlist )
    {
        testlist = testlist->next;
        ++count;
    }

    return count;
}

void SimpleTest_failTest ( const char * format, ... )
{
    va_list args;

    /* Output the failure to the console */
    va_start ( args, format );
    fprintf ( stderr, "FAILED : " );
    vfprintf ( stderr, format, args );
    fprintf ( stderr, "\n" );
    va_end ( args );

    longjmp ( g_simpleTest_jmpBuffer, 1 );
}

void SimpleTest_equalsTrue ( const char * file, int line, const char * xStr, int x )
{
    if ( ! x )
        SimpleTest_failTest ( "%s:%d : !( %s )", file, line, xStr );
    SimpleTest_log ( "%s:%d : %s == true", file, line, xStr );
}

void SimpleTest_equalsInt ( const char * file, int line, const char * xStr, const char * yStr, int64_t x, int64_t y )
{
    if ( x != y )
        SimpleTest_failTest ( "%s:%d : %s (%lld) != %s (%lld)", file, line, xStr, x, yStr, y );
    SimpleTest_log ( "%s:%d : %s (%lld) == %s (%lld)", file, line, xStr, x, yStr, y );
}

void SimpleTest_equalsFloat ( const char * file, int line, const char * xStr, const char * yStr, double x, double y, double epsilon )
{
    const double diff = fabs ( x - y );
    if ( diff > epsilon )
        SimpleTest_failTest ( "%s:%d : %s (%f) and %s (%f) differ by %f (limit is %f)", file, line, xStr, x, yStr, y, diff, epsilon );
    SimpleTest_log ( "%s:%d : %s (%f) and %s (%f) differ by %f (limit is %f)", file, line, xStr, x, yStr, y, diff, epsilon );
}

void SimpleTest_equalsMemory ( const char * file, int line, const char * xStr, const char * yStr, const void * x, int sizeX, const void * y, int sizeY )
{
    int index;
    const int8_t * xBytes = x,
                 * yBytes = y;

    if ( sizeX != sizeY )
        SimpleTest_failTest ( "%s:%d : %s != %s as size %d != size %d", file, line, xStr, yStr, sizeX, sizeY );

    if ( x && y )
    {
        for ( index = 0; index < sizeX; ++index )
            if ( xBytes [ index ] != yBytes [ index ] )
                SimpleTest_failTest ( "%s:%d %s != %s, difference at byte %d: 0x%2x != 0x%2x",
                                      file, line, xStr, yStr, index, xBytes [ index ], yBytes [ index ] );
    }
    else
    {
        if ( x != y )
            SimpleTest_failTest ( "%s:%d : %s != %s cannot be compared as one is null", file, line, xStr, yStr );
    }
    SimpleTest_log ( "%s:%d : %s (%d bytes) == %s (%d bytes)", file, line, xStr, sizeX, yStr, sizeY );
}

