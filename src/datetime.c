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

FudgeStatus FudgeDate_initialise ( FudgeDate * date,
                                   int32_t year,
                                   uint8_t month,
                                   uint8_t day )
{
    if ( ! date )
        return FUDGE_NULL_POINTER;

    if ( year < FUDGEDATE_MIN_YEAR || year > FUDGEDATE_MAX_YEAR )
        return FUDGE_DATETIME_INVALID_YEAR;
    date->year = year;

    if ( month > 12 )
        return FUDGE_DATETIME_INVALID_MONTH;
    date->month = month;

    /* TODO Make upper bound dependent on month (if set) */
    if ( day > 31 )
        return FUDGE_DATETIME_INVALID_DAY;
    date->day = day;

    return FUDGE_OK;
}

inline FudgeStatus FudgeTime_internalInitialise ( FudgeTime * time,
                                                  uint32_t seconds,
                                                  uint32_t nanoseconds,
                                                  FudgeDateTimePrecision precision,
                                                  fudge_bool hasTimezone,
                                                  int8_t timezoneOffset )
{
    if ( ! time )
        return FUDGE_NULL_POINTER;

    /* Adjust the time to match the precision */
    switch ( precision )
    {
        case FUDGE_DATETIME_PRECISION_MICROSECOND:  nanoseconds -= nanoseconds % 1000;      break;
        case FUDGE_DATETIME_PRECISION_MILLISECOND:  nanoseconds -= nanoseconds % 1000000;   break;
        case FUDGE_DATETIME_PRECISION_SECOND:       nanoseconds = 0;                        break;

        case FUDGE_DATETIME_PRECISION_MINUTE:
            nanoseconds = 0;
            seconds -= seconds % 60;
            break;

        case FUDGE_DATETIME_PRECISION_HOUR:
            nanoseconds = 0;
            seconds -= seconds % 3600;
            break;

        /* Nanoseconds is the highest precision - change nothing */
        case FUDGE_DATETIME_PRECISION_NANOSECOND:   break;

        /* Every precision below hour will render the time meaningless */
        case FUDGE_DATETIME_PRECISION_MILLENNIUM:
        case FUDGE_DATETIME_PRECISION_CENTURY:
        case FUDGE_DATETIME_PRECISION_YEAR:
        case FUDGE_DATETIME_PRECISION_MONTH:
        case FUDGE_DATETIME_PRECISION_DAY:
            seconds = nanoseconds = 0;
            break;
    }

    if ( seconds > FUDGETIME_MAX_SECONDS )
        return FUDGE_DATETIME_INVALID_SECONDS;
    if ( nanoseconds > FUDGETIME_MAX_NANOSECONDS )
        return FUDGE_DATETIME_INVALID_NANOSECONDS;
    time->seconds = seconds;
    time->nanoseconds = nanoseconds;
    time->precision = precision;
    time->hasTimezone = hasTimezone;
    time->timezoneOffset = timezoneOffset;
    return FUDGE_OK;
}

FudgeStatus FudgeTime_initialise ( FudgeTime * time,
                                   uint32_t seconds,
                                   uint32_t nanoseconds,
                                   FudgeDateTimePrecision precision )
{
    return FudgeTime_internalInitialise ( time,
                                          seconds,
                                          nanoseconds,
                                          precision,
                                          FUDGE_FALSE,
                                          0 );
}

FudgeStatus FudgeTime_initialiseWithTimezone ( FudgeTime * time,
                                               uint32_t seconds,
                                               uint32_t nanoseconds,
                                               FudgeDateTimePrecision precision,
                                               int8_t timezoneOffset )
{
    return FudgeTime_internalInitialise ( time,
                                          seconds,
                                          nanoseconds,
                                          precision,
                                          FUDGE_TRUE,
                                          timezoneOffset );
}

inline FudgeStatus FudgeDateTime_internalInitialise ( FudgeDateTime * datetime,
                                                      int32_t year,
                                                      uint8_t month,
                                                      uint8_t day,
                                                      uint32_t seconds,
                                                      uint32_t nanoseconds,
                                                      FudgeDateTimePrecision precision,
                                                      fudge_bool hasTimezone,
                                                      int8_t timezoneOffset )
{
    FudgeStatus status;

    /* If required - adjust the date values to match the required precision */
    switch ( precision )
    {
        case FUDGE_DATETIME_PRECISION_MILLENNIUM:
            year -= year % 1000;
            month = day = 0;
            break;

        case FUDGE_DATETIME_PRECISION_CENTURY:
            year -= year % 100;
            month = day = 0;
            break;

        case FUDGE_DATETIME_PRECISION_YEAR:
            month = day = 0;
            break;

        case FUDGE_DATETIME_PRECISION_MONTH:
            day = 0;
            break;

        /* The other precisions only affect time, leave them for the time
           initialiser function */
        default:
            break;
    }

    if ( ( status = FudgeDate_initialise ( &( datetime->date ),
                                           year,
                                           month,
                                           day ) ) != FUDGE_OK )
        return status;
    return FudgeTime_internalInitialise ( &( datetime->time ),
                                          seconds,
                                          nanoseconds,
                                          precision,
                                          hasTimezone,
                                          timezoneOffset );
}

FudgeStatus FudgeDateTime_initialise ( FudgeDateTime * datetime,
                                       int32_t year,
                                       uint8_t month,
                                       uint8_t day,
                                       uint32_t seconds,
                                       uint32_t nanoseconds,
                                       FudgeDateTimePrecision precision )
{
    return FudgeDateTime_internalInitialise ( datetime,
                                              year,
                                              month,
                                              day,
                                              seconds,
                                              nanoseconds,
                                              precision,
                                              FUDGE_FALSE,
                                              0 );
}

FudgeStatus FudgeDateTime_initialiseWithTimezone ( FudgeDateTime * datetime,
                                                   int32_t year,
                                                   uint8_t month,
                                                   uint8_t day,
                                                   uint32_t seconds,
                                                   uint32_t nanoseconds,
                                                   FudgeDateTimePrecision precision,
                                                   int8_t timezoneOffset )
{
    return FudgeDateTime_internalInitialise ( datetime,
                                              year,
                                              month,
                                              day,
                                              seconds,
                                              nanoseconds,
                                              precision,
                                              FUDGE_TRUE,
                                              timezoneOffset );
}

int FudgeDate_compare ( const FudgeDate * left, const FudgeDate * right )
{
    /* Handle null pointers */
    if ( ! left )
        return right ? -1 : 0;
    if ( ! right )
        return 1;

    /* Work down the significant elements until difference is found */
    if ( left->year != right->year )
        return left->year > right->year ? 1 : -1;
    if ( left->month != right->month )
        return left->month > right->month ? 1 : -1;
    if ( left->day != right->day )
        return left->day > right->day ? 1 : -1;
    return 0;
}

void FudgeTime_getTimeZoneAdjustedSeconds ( const FudgeTime * time, uint32_t * seconds, fudge_bool * previousDay )
{
    if ( time->hasTimezone )
    {
        const int adjusted = ( ( int ) time->timezoneOffset ) * 15 * 60;
        if ( adjusted < 0 )
        {
            if ( ( ( uint32_t ) ( -adjusted ) ) > time->seconds )
            {
                *seconds = 86400 - ( ( -adjusted ) - time->seconds );
                *previousDay = FUDGE_TRUE;
            }
            else
            {
                *seconds = time->seconds + adjusted;
                *previousDay = FUDGE_FALSE;
            }
        }
        else
        {
            *seconds = time->seconds + adjusted;
            *previousDay = FUDGE_FALSE;
        }
    }
    else
    {
        *seconds = time->seconds;
        *previousDay = FUDGE_FALSE;
    }
}

int FudgeTime_compare ( const FudgeTime * left, const FudgeTime * right )
{
    uint32_t leftSeconds, rightSeconds;
    fudge_bool leftPreviousDay, rightPreviousDay;

    /* Handle null pointer */
    if ( ! left )
        return right ? -1 : 0;
    if ( ! right )
        return 1;

    /* Adjust for timezone if required */
    FudgeTime_getTimeZoneAdjustedSeconds ( left, &leftSeconds, &leftPreviousDay );
    FudgeTime_getTimeZoneAdjustedSeconds ( right, &rightSeconds, &rightPreviousDay );

    /* Check for day differences */
    if ( leftPreviousDay && ! rightPreviousDay )
        return -1;
    if ( rightPreviousDay && ! leftPreviousDay )
        return 1;

    /* Now check the time values */
    if ( leftSeconds != rightSeconds )
        return leftSeconds > rightSeconds ? 1 : -1;
    if ( left->nanoseconds != right->nanoseconds )
        return left->nanoseconds > right->nanoseconds ? 1 : -1;
    return 0;
}

