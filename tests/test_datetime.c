/**
 * Copyright (C) 2010 - 2010, Vrai Stacey.
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
#include "fudge/datetime.h"
#include "simpletest.h"

DEFINE_TEST( Date )
    FudgeDate date1, date2;
    
    /* Test failure cases */
    TEST_EQUALS_INT( FudgeDate_initialise ( 0, 2010, 3, 15 ), FUDGE_NULL_POINTER );
    TEST_EQUALS_INT( FudgeDate_initialise ( &date1, 8388608, 3, 15 ), FUDGE_DATETIME_INVALID_YEAR );
    TEST_EQUALS_INT( FudgeDate_initialise ( &date2, -8388608, 1, 12 ), FUDGE_DATETIME_INVALID_YEAR );
    TEST_EQUALS_INT( FudgeDate_initialise ( &date1, 2010, 13, 30 ), FUDGE_DATETIME_INVALID_MONTH );
    TEST_EQUALS_INT( FudgeDate_initialise ( &date2, 2010, 6, 32 ), FUDGE_DATETIME_INVALID_DAY );

    /* Test boundaries */
    TEST_EQUALS_INT( FudgeDate_initialise ( &date1, FUDGEDATE_MAX_YEAR, 12, 31 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeDate_initialise ( &date2, FUDGEDATE_MIN_YEAR, 0, 0 ), FUDGE_OK );

    /* Test construction and comparison */
    TEST_EQUALS_INT( FudgeDate_initialise ( &date1, 1688, 10, 16 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeDate_initialise ( &date2, 1688, 10, 16 ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeDate_compare ( 0, 0 ), 0 );
    TEST_EQUALS_INT( FudgeDate_compare ( &date1, 0 ), 1 );
    TEST_EQUALS_INT( FudgeDate_compare ( 0, &date2 ), -1 );
    TEST_EQUALS_INT( FudgeDate_compare ( &date1, &date2 ), 0 );
    
    TEST_EQUALS_INT( FudgeDate_initialise ( &date1, 1688, 10, 15 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeDate_compare ( &date1, &date2 ), -1 );
    TEST_EQUALS_INT( FudgeDate_compare ( &date2, &date1 ), 1 );

    TEST_EQUALS_INT( FudgeDate_initialise ( &date2, 1688, 9, 15 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeDate_compare ( &date1, &date2 ), 1 );
    TEST_EQUALS_INT( FudgeDate_compare ( &date2, &date1 ), -1 );

    TEST_EQUALS_INT( FudgeDate_initialise ( &date1, 1687, 9, 15 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeDate_compare ( &date1, &date2 ), -1 );
    TEST_EQUALS_INT( FudgeDate_compare ( &date2, &date1 ), 1 );
END_TEST

DEFINE_TEST( Time )
    FudgeTime time1, time2;

    /* Test failure cases */
    TEST_EQUALS_INT( FudgeTime_initialise ( 0, 10000, 10000, FUDGE_DATETIME_PRECISION_NANOSECOND ), FUDGE_NULL_POINTER );
    TEST_EQUALS_INT( FudgeTime_initialiseWithTimezone ( 0, 20000, 20000, FUDGE_DATETIME_PRECISION_NANOSECOND, 0 ), FUDGE_NULL_POINTER );
    TEST_EQUALS_INT( FudgeTime_initialise ( &time1, FUDGETIME_MAX_SECONDS + 1, 0, FUDGE_DATETIME_PRECISION_NANOSECOND ), FUDGE_DATETIME_INVALID_SECONDS );
    TEST_EQUALS_INT( FudgeTime_initialise ( &time2, FUDGETIME_MAX_SECONDS, FUDGETIME_MAX_NANOSECONDS + 1, FUDGE_DATETIME_PRECISION_NANOSECOND ), FUDGE_DATETIME_INVALID_NANOSECONDS );

    /* Test boundaries */
    TEST_EQUALS_INT( FudgeTime_initialise ( &time1, 0, 0, FUDGE_DATETIME_PRECISION_MILLENNIUM ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeTime_initialise ( &time2, FUDGETIME_MAX_SECONDS, 1000000000, FUDGE_DATETIME_PRECISION_NANOSECOND ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeTime_initialiseWithTimezone ( &time1, 0, 0, FUDGE_DATETIME_PRECISION_MILLENNIUM, -48 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeTime_initialiseWithTimezone ( &time1, FUDGETIME_MAX_SECONDS, 1000000000, FUDGE_DATETIME_PRECISION_NANOSECOND, 48 ), FUDGE_OK );

    /* Test precision - using 15:49:45.123456789 as a base, check the effect
       of reducing precision */
    TEST_EQUALS_INT( FudgeTime_initialise ( &time1, 56985, 123456789, FUDGE_DATETIME_PRECISION_NANOSECOND ), FUDGE_OK );
    TEST_EQUALS_TIME( time1, 56985, 123456789, FUDGE_DATETIME_PRECISION_NANOSECOND, -128 );
    TEST_EQUALS_INT( FudgeTime_initialise ( &time1, 56985, 123456789, FUDGE_DATETIME_PRECISION_MICROSECOND ), FUDGE_OK );
    TEST_EQUALS_TIME( time1, 56985, 123456000, FUDGE_DATETIME_PRECISION_MICROSECOND, -128 );
    TEST_EQUALS_INT( FudgeTime_initialise ( &time1, 56985, 123456789, FUDGE_DATETIME_PRECISION_MILLISECOND ), FUDGE_OK );
    TEST_EQUALS_TIME( time1, 56985, 123000000, FUDGE_DATETIME_PRECISION_MILLISECOND, -128 );
    TEST_EQUALS_INT( FudgeTime_initialise ( &time1, 56985, 123456789, FUDGE_DATETIME_PRECISION_SECOND ), FUDGE_OK );
    TEST_EQUALS_TIME( time1, 56985, 0, FUDGE_DATETIME_PRECISION_SECOND, -128 );
    TEST_EQUALS_INT( FudgeTime_initialise ( &time1, 56985, 123456789, FUDGE_DATETIME_PRECISION_MINUTE ), FUDGE_OK );
    TEST_EQUALS_TIME( time1, 56940, 0, FUDGE_DATETIME_PRECISION_MINUTE, -128 );
    TEST_EQUALS_INT( FudgeTime_initialise ( &time1, 56985, 123456789, FUDGE_DATETIME_PRECISION_HOUR ), FUDGE_OK );
    TEST_EQUALS_TIME( time1, 54000, 0, FUDGE_DATETIME_PRECISION_HOUR, -128 );
    TEST_EQUALS_INT( FudgeTime_initialise ( &time1, 56985, 123456789, FUDGE_DATETIME_PRECISION_DAY ), FUDGE_OK );
    TEST_EQUALS_TIME( time1, 0, 0, FUDGE_DATETIME_PRECISION_DAY, -128 );

    /* Test construction and comparison */
    TEST_EQUALS_INT( FudgeTime_initialise ( &time1, 72057, 500000000, FUDGE_DATETIME_PRECISION_NANOSECOND ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeTime_initialise ( &time2, 72057, 500000000, FUDGE_DATETIME_PRECISION_NANOSECOND ), FUDGE_OK );

    TEST_EQUALS_INT( FudgeTime_compare ( 0, 0 ), 0 );
    TEST_EQUALS_INT( FudgeTime_compare ( &time1, 0 ), 1 );
    TEST_EQUALS_INT( FudgeTime_compare ( 0, &time2 ), -1 );
    TEST_EQUALS_INT( FudgeTime_compare ( &time1, &time2 ), 0 );

    TEST_EQUALS_INT( FudgeTime_initialise ( &time1, 72057, 499999999, FUDGE_DATETIME_PRECISION_NANOSECOND ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeTime_compare ( &time1, &time2 ), -1 );
    TEST_EQUALS_INT( FudgeTime_compare ( &time2, &time1 ), 1 );

    TEST_EQUALS_INT( FudgeTime_initialise ( &time2, 72056, 499999999, FUDGE_DATETIME_PRECISION_NANOSECOND ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeTime_compare ( &time1, &time2 ), 1 );
    TEST_EQUALS_INT( FudgeTime_compare ( &time2, &time1 ), -1 );

    /* Test timezone comparisons */
    TEST_EQUALS_INT( FudgeTime_initialise ( &time1, 30600, 0, FUDGE_DATETIME_PRECISION_NANOSECOND ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeTime_initialiseWithTimezone ( &time2, 30600, 0, FUDGE_DATETIME_PRECISION_NANOSECOND, -8 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeTime_compare ( &time1, &time2 ), 1 );

    TEST_EQUALS_INT( FudgeTime_initialise ( &time1, 23400, 0, FUDGE_DATETIME_PRECISION_NANOSECOND ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeTime_compare ( &time1, &time2 ), 0 );

    TEST_EQUALS_INT( FudgeTime_initialiseWithTimezone ( &time2, 30600, 0, FUDGE_DATETIME_PRECISION_NANOSECOND, -46 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeTime_compare ( &time1, &time2 ), 1 );

    TEST_EQUALS_INT( FudgeTime_initialise ( &time1, 75600, 0, FUDGE_DATETIME_PRECISION_NANOSECOND ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeTime_compare ( &time1, &time2 ), 1 );     /* Same time - but time2 is referring to the previous day */

    TEST_EQUALS_INT( FudgeTime_initialiseWithTimezone ( &time1, 0, 0, FUDGE_DATETIME_PRECISION_NANOSECOND, -12 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeTime_compare ( &time1, &time2 ), 0 );

    TEST_EQUALS_INT( FudgeTime_initialise ( &time1, 75600, 0, FUDGE_DATETIME_PRECISION_NANOSECOND ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeTime_initialiseWithTimezone ( &time2, 32400, 0, FUDGE_DATETIME_PRECISION_NANOSECOND, 48 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeTime_compare ( &time1, &time2 ), 0 );
END_TEST

DEFINE_TEST( DateTime )
    FudgeDateTime datetime;

    /* Test failure cases */
    TEST_EQUALS_INT( FudgeDateTime_initialise ( 0, 2010, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_NANOSECOND ), FUDGE_NULL_POINTER );
    TEST_EQUALS_INT( FudgeDateTime_initialise ( &datetime, 8388608, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_NANOSECOND ), FUDGE_DATETIME_INVALID_YEAR );
    TEST_EQUALS_INT( FudgeDateTime_initialise ( &datetime, -8388608, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_NANOSECOND ), FUDGE_DATETIME_INVALID_YEAR );
    TEST_EQUALS_INT( FudgeDateTime_initialise ( &datetime, 2010, 13, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_NANOSECOND ), FUDGE_DATETIME_INVALID_MONTH );
    TEST_EQUALS_INT( FudgeDateTime_initialise ( &datetime, 2010, 3, 32, 62257, 1020408010, FUDGE_DATETIME_PRECISION_NANOSECOND ), FUDGE_DATETIME_INVALID_DAY );
    TEST_EQUALS_INT( FudgeDateTime_initialiseWithTimezone ( 0, 2010, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_NANOSECOND, 0 ), FUDGE_NULL_POINTER );
    TEST_EQUALS_INT( FudgeDateTime_initialiseWithTimezone ( &datetime, 2010, 3, 20, FUDGETIME_MAX_SECONDS + 1, 1020408010, FUDGE_DATETIME_PRECISION_NANOSECOND, 0 ), FUDGE_DATETIME_INVALID_SECONDS );
    TEST_EQUALS_INT( FudgeDateTime_initialiseWithTimezone ( &datetime, 2010, 3, 20, 62257, FUDGETIME_MAX_NANOSECONDS + 1, FUDGE_DATETIME_PRECISION_NANOSECOND, 0 ), FUDGE_DATETIME_INVALID_NANOSECONDS );

    /* Test precision - 2010/3/2 17:17:37.1020408010UTC+1 is the base value */
    TEST_EQUALS_INT( FudgeDateTime_initialiseWithTimezone ( &datetime, 1997, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_NANOSECOND, 4 ), FUDGE_OK );
    TEST_EQUALS_DATE( datetime.date, 1997, 3, 20 );  TEST_EQUALS_TIME( datetime.time, 62257, 1020408010, FUDGE_DATETIME_PRECISION_NANOSECOND, 4 );
    TEST_EQUALS_INT( FudgeDateTime_initialiseWithTimezone ( &datetime, 1997, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_MICROSECOND, 4 ), FUDGE_OK );
    TEST_EQUALS_DATE( datetime.date, 1997, 3, 20 );  TEST_EQUALS_TIME( datetime.time, 62257, 1020408000, FUDGE_DATETIME_PRECISION_MICROSECOND, 4 );
    TEST_EQUALS_INT( FudgeDateTime_initialiseWithTimezone ( &datetime, 1997, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_MILLISECOND, 4 ), FUDGE_OK );
    TEST_EQUALS_DATE( datetime.date, 1997, 3, 20 );  TEST_EQUALS_TIME( datetime.time, 62257, 1020000000, FUDGE_DATETIME_PRECISION_MILLISECOND, 4 );
    TEST_EQUALS_INT( FudgeDateTime_initialiseWithTimezone ( &datetime, 1997, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_SECOND, 4 ), FUDGE_OK );
    TEST_EQUALS_DATE( datetime.date, 1997, 3, 20 );  TEST_EQUALS_TIME( datetime.time, 62257, 0, FUDGE_DATETIME_PRECISION_SECOND, 4 );
    TEST_EQUALS_INT( FudgeDateTime_initialiseWithTimezone ( &datetime, 1997, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_MINUTE, 4 ), FUDGE_OK );
    TEST_EQUALS_DATE( datetime.date, 1997, 3, 20 );  TEST_EQUALS_TIME( datetime.time, 62220, 0, FUDGE_DATETIME_PRECISION_MINUTE, 4 );
    TEST_EQUALS_INT( FudgeDateTime_initialiseWithTimezone ( &datetime, 1997, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_HOUR, 4 ), FUDGE_OK );
    TEST_EQUALS_DATE( datetime.date, 1997, 3, 20 );  TEST_EQUALS_TIME( datetime.time, 61200, 0, FUDGE_DATETIME_PRECISION_HOUR, 4 );
    TEST_EQUALS_INT( FudgeDateTime_initialiseWithTimezone ( &datetime, 1997, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_DAY, 4 ), FUDGE_OK );
    TEST_EQUALS_DATE( datetime.date, 1997, 3, 20 );  TEST_EQUALS_TIME( datetime.time, 0, 0, FUDGE_DATETIME_PRECISION_DAY, 4 );
    TEST_EQUALS_INT( FudgeDateTime_initialiseWithTimezone ( &datetime, 1997, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_MONTH, 4 ), FUDGE_OK );
    TEST_EQUALS_DATE( datetime.date, 1997, 3, 0 );  TEST_EQUALS_TIME( datetime.time, 0, 0, FUDGE_DATETIME_PRECISION_MONTH, 4 );
    TEST_EQUALS_INT( FudgeDateTime_initialiseWithTimezone ( &datetime, 1997, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_YEAR, 4 ), FUDGE_OK );
    TEST_EQUALS_DATE( datetime.date, 1997, 0, 0 );  TEST_EQUALS_TIME( datetime.time, 0, 0, FUDGE_DATETIME_PRECISION_YEAR, 4 );
    TEST_EQUALS_INT( FudgeDateTime_initialiseWithTimezone ( &datetime, 1997, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_CENTURY, 4 ), FUDGE_OK );
    TEST_EQUALS_DATE( datetime.date, 1900, 0, 0 );  TEST_EQUALS_TIME( datetime.time, 0, 0, FUDGE_DATETIME_PRECISION_CENTURY, 4 );
    TEST_EQUALS_INT( FudgeDateTime_initialiseWithTimezone ( &datetime, 1997, 3, 20, 62257, 1020408010, FUDGE_DATETIME_PRECISION_MILLENNIUM, 4 ), FUDGE_OK );
    TEST_EQUALS_DATE( datetime.date, 1000, 0, 0 );  TEST_EQUALS_TIME( datetime.time, 0, 0, FUDGE_DATETIME_PRECISION_MILLENNIUM, 4 );
END_TEST

DEFINE_TEST_SUITE( DateTime )
    REGISTER_TEST( Date )
    REGISTER_TEST( Time )
    REGISTER_TEST( DateTime )
END_TEST_SUITE

