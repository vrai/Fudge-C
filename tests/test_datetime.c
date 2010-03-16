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
    TEST_EQUALS_INT( FudgeTime_initialise ( &time1, 86400, 0, FUDGE_DATETIME_PRECISION_NANOSECOND ), FUDGE_DATETIME_INVALID_SECONDS );

    /* Test boundaries */
    TEST_EQUALS_INT( FudgeTime_initialise ( &time1, 0, 0, FUDGE_DATETIME_PRECISION_MILLENNIUM ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeTime_initialise ( &time2, FUDGETIME_MAX_SECONDS, 1000000000, FUDGE_DATETIME_PRECISION_NANOSECOND ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeTime_initialiseWithTimezone ( &time1, 0, 0, FUDGE_DATETIME_PRECISION_MILLENNIUM, -48 ), FUDGE_OK );
    TEST_EQUALS_INT( FudgeTime_initialiseWithTimezone ( &time1, FUDGETIME_MAX_SECONDS, 1000000000, FUDGE_DATETIME_PRECISION_NANOSECOND, 48 ), FUDGE_OK );

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

DEFINE_TEST_SUITE( DateTime )
    REGISTER_TEST( Date )
    REGISTER_TEST( Time )
END_TEST_SUITE
