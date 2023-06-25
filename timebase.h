/*************************************************
Copyright (C), 2018-2028, YWS. Co., Ltd.
File name: timebase.h
Author:
Version:
Custom version:
Date:
Description:
        handle the timebase, load the task into queue for schedular;


Others:
Function List:
        void TimeBaseHandle(void)
            it is called in main.c

        static inline Ticks_t scehdular_ElapsedTicket(Ticks_t *p_start_time)
            This function can calculat the elapsed time from last calling to now

1. History:

2. Date:
Author:
Modification:
***************************************************/

#ifndef _TIMEBASE_H
#define _TIMEBASE_H

#include "n32g430_tim.h"

/*The longest time:(2^16)*1/48MHZ*/
#define SCHEDULAR_COUNTER_MSK 0x0000FFFF
/*HPTSC Config*/
#define CONFIG_SCHEDULAR_HZ 48000000 // select timer3    96Mhz
// #define CONFIG_SCHEDULAR_HZ   72000000  //select timer3   144Mhz
typedef unsigned long Ticks_t;

#if !defined CONFIG_SCHEDULAR_HZ
#error "CONFIG_SCHEDULAR_HZ must be defined"
#endif

#if !defined SCHEDULAR_COUNTER_MSK
#error "Low level driver must be include"
#endif

#define SCHEDULAR_TIMER_STOPPED 0U
#define SCHEDULAR_TIMER_EXPIRED 1U

static inline Ticks_t schedular_Timestamp(void)
{
    Ticks_t now = TIM3->CNT;

    /*
     * The systick timer is a down counter, but we have to return
     * an up-counting timestamp.
     */
    return SCHEDULAR_COUNTER_MSK - now;
}

/* -------------------------------------------------------------------- */
/* Pure timestamp related functions.                                    */
/* -------------------------------------------------------------------- */

/**
 * Round a floating point value to an integer type at compile time.
 *
 * \param[in] type The target data type.
 * \param[in] val  The floating point value to round to the given type.
 *
 * \returns Rounded value cast to the desired type.
 */
#define ROUND_TO(type, val) \
    ((type)((val) < 0 ? (val)-0.5 : (val) + 0.5))

/**
 * Convert time in [sec] into ticks, target data type specified by user.
 *
 * \param[in] type The data type used to hold the resulting ticks.
 * \param[in] sec Time in [sec].
 *
 * \returns
 * Number of ticks representing the given time.
 */
#define schedular_SecToTicksUserType(type, sec) \
    ROUND_TO(type, (double)CONFIG_SCHEDULAR_HZ *(sec))

/**
 * Convert time in [ms] into ticks, target data type specified by user.
 *
 * \param[in] type The data type used to hold the resulting ticks.
 * \param[in] ms Time in [ms].
 *
 * \returns
 * Number of ticks representing the given time.
 */
#define schedular_MsToTicksUserType(type, ms) \
    ROUND_TO(type, (double)CONFIG_SCHEDULAR_HZ *(ms) / 1000.0)

/**
 * Convert time in [us] into ticks, target data type specified by user.
 *
 * \param[in] type The data type used to hold the resulting ticks.
 * \param[in] us Time in [us].
 *
 * \returns
 * Number of ticks representing the given time.
 */
#define schedular_UsToTicksUserType(type, us) \
    ROUND_TO(type, (double)CONFIG_SCHEDULAR_HZ *(us) / 1000000.0)

/**
 * Convert time in [sec] into ticks, target data type Ticks_t.
 *
 * \param[in] sec Time in [sec].
 *
 * \returns
 * Number of ticks representing the given time.
 */
#define schedular_SecToTicks(sec) schedular_SecToTicksUserType(Ticks_t, sec)

/**
 * Convert time in [ms] into ticks, target data type Ticks_t.
 *
 * \param[in] ms Time in [ms].
 *
 * \returns
 * Number of ticks representing the given time.
 */
#define schedular_MsToTicks(ms) schedular_MsToTicksUserType(Ticks_t, ms)

/**
 * Convert time in [us] into ticks, target data type Ticks_t.
 *
 * \param[in] us Time in [us].
 *
 * \returns
 * Number of ticks representing the given time.
 */
#define schedular_UsToTicks(us) schedular_UsToTicksUserType(Ticks_t, us)

/**
 * Gives the time elapsed between two timestamps.
 *
 * \param[in] ts_older
 *      The timestamp which is the older one of the two timestamps.
 * \param[in] ts_newer
 *      The timestamp which is the newer one of the two timestamps.
 */
static inline Ticks_t schedular_Elapsed(
    Ticks_t ts_older, Ticks_t ts_newer)
{
    return ((ts_newer - ts_older) & SCHEDULAR_COUNTER_MSK);
}

/**
 * Test if a given period of time is elapsed.
 */
static inline bool scehdular_IsElapsed(
    Ticks_t start_time, Ticks_t time_span)
{
    Ticks_t now = schedular_Timestamp();
    return (schedular_Elapsed(start_time, now) >= time_span);
}

/**
 * Gives the time elapsed from last.
 *
 * \param[in] p_start_time
 *      Elapsed ticket
 */
static inline Ticks_t scehdular_ElapsedTicket(
    Ticks_t *p_start_time)
{
    Ticks_t now = schedular_Timestamp();
    if (*p_start_time == 0) // Restart counter
    {
        *p_start_time = now;
    }
    Ticks_t st_Elapsed = 0;
    st_Elapsed = schedular_Elapsed(*p_start_time, now);
    *p_start_time = now;
    return (st_Elapsed & SCHEDULAR_COUNTER_MSK);
}

/**
 * Test if a period of time is elapsed and update start timestamp for
 * repetitive timer user.
 */
static inline bool schedular_IsElapsedRepetitive(
    Ticks_t *p_start_time, Ticks_t time_span)
{
    Ticks_t now = schedular_Timestamp();

    if (schedular_Elapsed(*p_start_time, now) >= time_span)
    {
        *p_start_time = now;
        return true;
    }
    return false;
}

/**
 * Delay execution for a certain number of ticks, "delay" use by schedular_UsToTicks(x)
 */
static inline void schedular_Delay(Ticks_t delay)
{
    Ticks_t start = schedular_Timestamp();
    Ticks_t now;
    do
    {
        now = schedular_Timestamp();
    } while (schedular_Elapsed(start, now) < delay);
}

/**
 * Define uninitialized timer based on timestamp counter.
 *
 * \param[in] type
 *      The type of the variable used to hold the timer value.
 *      It must be an unsigned type big enough for the desired time span.
 * \param[in] name
 *      Name of the timer.
 *
 * \note
 * It is up to the user to choose the data type used for the timer value.
 * Therefore we provide this feature as macro instead of an
 * inline function.
 */
#define schedular_DefineUninitializedTimer(type, name) \
    struct                                             \
    {                                                  \
        type value;                                    \
        Ticks_t ts_last;                               \
    } name

/**
 * Constant expression to initialize timer based on timestamp as stopped.
 */
// #define SCHEDULAR_TIMER_STOPPED 0U
#define SCHEDULAR_TIMER_INIT_VAL   \
    {                              \
        SCHEDULAR_TIMER_STOPPED, 0 \
    }

/**
 * Define timer based on timestamp counter initialized as stopped.
 *
 * \param[in] type
 *      The type of the variable used to hold the timer value.
 *      It must be an unsigned type big enough for the desired time span.
 * \param[in] name
 *      Name of the timer.
 *
 * \note
 * It is up to the user to choose the data type used for the timer value.
 * Therefore we provide this feature as macro instead of an
 * inline function.
 */
#define schedular_DefineTimer(type, name) \
    schedular_DefineUninitializedTimer(type, name) = SCHEDULAR_TIMER_INIT_VAL;

/**
 * Load timer based on timestamp counter with new tick value.
 *
 * \param[in,out] p_tim
 *      Pointer to timer variable created with \a hptsc_DefTimerVar.
 * \param[in] ticks
 *      Period of time to load the timer with.
 */
#define schedular_LoadTimer(p_tim, ticks)                     \
    do                                                        \
    {                                                         \
        (p_tim)->value = ((ticks) + SCHEDULAR_TIMER_EXPIRED); \
        (p_tim)->ts_last = schedular_Timestamp();             \
    } while (0)

/**
 * Check if timer based on timestamp counter has expired.
 *
 * \param[in] p_tim
 *      Pointer to timer variable created with \a hptsc_DefTimerVar.
 *
 * \note
 * It is up to the user to choose the data type used for the timer value.
 * Therefore we provide this feature as macro instead of an
 */
#define schedular_IsTimerExpired(p_tim) \
    ((p_tim)->value == SCHEDULAR_TIMER_EXPIRED)

/**
 * Check if timer based on timestamp counter is stopped.
 *
 * \param[in] p_tim
 *      Pointer to timer variable created with \a hptsc_DefTimerVar.
 *
 * \note
 * It is up to the user to choose the data type used for the timer value.
 * Therefore we provide this feature as macro instead of an
 */
#define schedular_IsTimerStopped(p_tim) \
    ((p_tim)->value == SCHEDULAR_TIMER_STOPPED)

/**
 * Check if timer based on timestamp counter is running.
 *
 * \param[in] p_tim
 *      Pointer to timer variable created with \a hptsc_DefTimerVar.
 *
 * \note
 * It is up to the user to choose the data type used for the timer value.
 * Therefore we provide this feature as macro instead of an
 */
// #define HPTSC_TIMER_EXPIRED 1U
#define schedular_IsTimerRunning(p_tim) \
    ((p_tim)->value > SCHEDULAR_TIMER_EXPIRED)

/**
 * Update timer based on timestamp counter.
 *
 * This macro uses the timestamp coutner to measure the time elapsed
 * since last invocation and updates the timer value accordingly. The
 * time between two invocations must be less than the time duration
 * provided by the High Precision Timestamp Counter minus 2 ticks.
 *
 * \param[in,out] p_tim
 *      Pointer to timer variable created with \a hptsc_DefTimerVar.
 *
 * \note
 * It is up to the user to choose the data type used for the timer value.
 * Therefore we provide this feature as macro instead of an
 */
#define schedular_UpdateTimer(p_tim)                                 \
    do                                                               \
    {                                                                \
        if (schedular_IsTimerRunning(p_tim))                         \
        {                                                            \
            Ticks_t now = schedular_Timestamp();                     \
            Ticks_t elapsed =                                        \
                schedular_Elapsed((p_tim)->ts_last, now);            \
            (p_tim)->value =                                         \
                (p_tim)->value > (elapsed + SCHEDULAR_TIMER_EXPIRED) \
                    ? (p_tim)->value - elapsed                       \
                    : SCHEDULAR_TIMER_EXPIRED;                       \
            (p_tim)->ts_last = now;                                  \
        }                                                            \
    } while (0)

#endif /* _TIMEBASE_H */
