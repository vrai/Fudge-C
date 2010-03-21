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
#ifndef INC_FUDGE_SIMPLETEST_H
#define INC_FUDGE_SIMPLETEST_H

#include "fudge/fudge.h"
#include "fudge/types.h"
#include <stdio.h>
#ifdef FUDGE_HAVE_SETJMP_H
#include <setjmp.h>
#endif /* ifdef FUDGE_HAVE_SETJMP_H */

/* Used by the error handlers to jump out the test */
extern jmp_buf g_simpleTest_jmpBuffer;

/* Test macros - perform the actual test validations */
#define TEST_EQUALS_TRUE( x ) SimpleTest_equalsTrue ( __FILE__, __LINE__, #x, x )
#define TEST_EQUALS_INT( x, y ) SimpleTest_equalsInt ( __FILE__, __LINE__, #x, #y, x, y )
#define TEST_EQUALS_FLOAT( x, y, epsilon ) SimpleTest_equalsFloat ( __FILE__, __LINE__, #x, #y, x, y, epsilon )
#define TEST_EQUALS_MEMORY( x, sx, y, sy ) SimpleTest_equalsMemory ( __FILE__, __LINE__, #x, #y, x, sx, y, sy )
#define TEST_EQUALS_DATE( x, yy, ym, yd ) SimpleTest_equalsDate ( __FILE__, __LINE__, #x, x, yy, ym, yd )
#define TEST_EQUALS_TIME( x, ys, yn, yp, yt ) SimpleTest_equalsTime ( __FILE__, __LINE__, #x, x, ys, yn, yp, yt )

/* Helper macros - the test framework should be accessed using these */
#define DEFINE_TEST_SUITE( name )                               \
    int main ( int argc, char * argv [] )                       \
    {                                                           \
        SimpleTest_initialiseSuite ( #name );                   \
        atexit ( SimpleTest_cleanupSuite);

#define END_TEST_SUITE                                          \
        return SimpleTest_runSuite ( );                         \
    }

#define DEFINE_TEST( name )                                     \
    int SimpleTest_test_##name ( )                              \
    {                                                           \
        int _jmpReturnValue = setjmp ( g_simpleTest_jmpBuffer );\
        if ( _jmpReturnValue )                                  \
            return _jmpReturnValue;                             \
        else                                                    \
        {                                                       \
            TEST_EQUALS_INT( Fudge_init ( ), FUDGE_OK );        \
            {


#define END_TEST                                                \
            }                                                   \
        }                                                       \
        return 0;                                               \
    }

#define REGISTER_TEST( name )                                   \
    SimpleTest_registerTest ( #name, SimpleTest_test_##name );

/* Test frame implementation - don't call these directly, use the macros */
extern void SimpleTest_initialiseSuite ( const char * name );
extern int SimpleTest_runSuite ( );
extern void SimpleTest_cleanupSuite ( );

extern void SimpleTest_registerTest ( const char * name, int ( *test ) ( ) );
extern int SimpleTest_countTests ( );

extern void SimpleTest_equalsTrue ( const char * file, int line, const char * xStr, int x );
extern void SimpleTest_equalsInt ( const char * file, int line, const char * xStr, const char * yStr, int64_t x, int64_t y );
extern void SimpleTest_equalsFloat ( const char * file, int line, const char * xStr, const char * yStr, double x, double y, double epsilon );
extern void SimpleTest_equalsMemory ( const char * file, int line, const char * xStr, const char * yStr, const void * x, int sizeX, const void * y, int sizeY );
extern void SimpleTest_equalsDate ( const char * file, int line, const char * dateStr, FudgeDate date, int32_t year, uint8_t month, uint8_t day );
extern void SimpleTest_equalsTime ( const char * file, int line, const char * timeStr, FudgeTime time, uint32_t seconds, uint32_t nanoseconds, FudgeDateTimePrecision precision, int timezone );

#endif

