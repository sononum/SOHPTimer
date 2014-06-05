//
//  so_hptimer.h
//  SOHPTimer
//
//  Created by Ulrich Zurucker on 05.06.14.
//  Copyright (c) 2014 sononum.net. All rights reserved.
//

#ifndef SOHPTimer_so_hptimer_h
#define SOHPTimer_so_hptimer_h

#include <CoreFoundation/CoreFoundation.h>

typedef struct so_hptimer_s* so_hptimer;

typedef void (^so_hptimer_action)();

/**
 Creates a new instance of the timer, the time interval is in nano-seconds
 */
OSStatus so_hptimer_create(so_hptimer* timer,
                           uint64_t timeInterval);

/**
 set the time interval in nano-seconds
 */
OSStatus so_hptimer_set_interval(so_hptimer timer,
                                 uint64_t timeInterval);

/**
 get the time interval in nano-seconds
 */
OSStatus so_hptimer_get_interval(const so_hptimer timer,
                                 uint64_t* outTimeInterval);

/**
 The work amount is a real number ]0..1[ indicating the expected ratio of
 idle time and wait time, e.g. timer interval is 100ms, work is 1ms gives a
 work amount of 0.01.
 Must be set when timer is not running. See thread_time_constraint_policy_data_t
 and implementation for details.
 */
OSStatus so_hptimer_set_workamount(so_hptimer timer, double workAmount);

/**
 Set the max number of timer repetitions
 */
OSStatus so_hptimer_set_maxrepetitions(so_hptimer timer,
                                       uint64_t maxRepetitions);

/**
 Waits until the timer has stopped, only useful for timers with a repetition
 limit set
 */
OSStatus so_hptimer_waituntilfinished(const so_hptimer timer);

/**
 Start the timer to invoke its action repating with its interval.
 The action is performed on a real time thread therefor *must* finish fast.
 */
OSStatus so_hptimer_resume(so_hptimer timer);

/**
 Suspend the execution of the timer
 */
OSStatus so_hptimer_suspend(so_hptimer timer);

/**
 Dispose the timer
 */
OSStatus so_hptimer_dispose(so_hptimer timer);

/*
 Set the block to be executed when the timer fires
 */
OSStatus so_hptimer_set_action(so_hptimer timer, so_hptimer_action action);

#endif
