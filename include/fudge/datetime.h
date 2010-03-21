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
#ifndef INC_FUDGE_DATETIME_H
#define INC_FUDGE_DATETIME_H

#include "fudge/status.h"
#include "fudge/types.h"

#ifdef __cplusplus
    extern "C" {
#endif

/* Interim date/time implementation. Will be replaced when the Fudge date/time
   standard is finalised.

   For more details see: http://wiki.fudgemsg.org/display/FDG/DateTime */

/* See types.h for acceptable ranges of year, month and day */
FUDGEAPI FudgeStatus FudgeDate_initialise ( FudgeDate * date,
                                            int32_t year,
                                            uint8_t month,
                                            uint8_t day );

/* See types.h for acceptable range for seconds and nanoseconds.
   Values will be rounded to their nearest precision as required. */
FUDGEAPI FudgeStatus FudgeTime_initialise ( FudgeTime * time,
                                            uint32_t seconds,
                                            uint32_t nanoseconds,
                                            FudgeDateTimePrecision precision );

FUDGEAPI FudgeStatus FudgeTime_initialiseWithTimezone ( FudgeTime * time,
                                                        uint32_t seconds,
                                                        uint32_t nanoseconds,
                                                        FudgeDateTimePrecision precision,
                                                        int8_t timezoneOffset );

/* Combination of the date and time specific initialisation functions. The
   only difference is that the precision will be used to round the date value
   if required, as well as the time value. */
FUDGEAPI FudgeStatus FudgeDateTime_initialise ( FudgeDateTime * datetime,
                                                int32_t year,
                                                uint8_t month,
                                                uint8_t day,
                                                uint32_t seconds,
                                                uint32_t nanoseconds,
                                                FudgeDateTimePrecision precision );

FUDGEAPI FudgeStatus FudgeDateTime_initialiseWithTimezone ( FudgeDateTime * datetime,
                                                            int32_t year,
                                                            uint8_t month,
                                                            uint8_t day,
                                                            uint32_t seconds,
                                                            uint32_t nanoseconds,
                                                            FudgeDateTimePrecision precision,
                                                            int8_t timezoneOffset );

/* Compares the two dates, working through year, month and day until a
   difference is found (if any). A null pointer is consider to be a lower
   values than any valid date. Returns 0 if the two dates are identical, 1
   if the left value is greater than the right, otherwise -1. */
FUDGEAPI int FudgeDate_compare ( const FudgeDate * left, const FudgeDate * right );

/* As the date comparison, but for time. Adjusts for timezones (if present),
   so two times with different literal values could be considered identical. */
FUDGEAPI int FudgeTime_compare ( const FudgeTime * left, const FudgeTime * right );

#ifdef __cplusplus
    }
#endif

#endif

