//
//  so_hptimer.c
//  SOHPTimer
//
//  Created by Ulrich Zurucker on 05.06.14.
//  Copyright (c) 2014 sononum.net. All rights reserved.
//

#include "so_hptimer.h"
#include <pthread.h>
#include <mach/mach.h>
#include <mach/mach_time.h>

static struct mach_timebase_info _timebase_info;

typedef struct so_hptimer_s {
    uint64_t            mTimeInterval;
    so_hptimer_action   mAction;
    void*               mRefCon;
    pthread_t           mThread;
    int                 mRun;
} so_hptimer_s;

static void _move_pthread_to_realtime_scheduling_class() {
    const uint64_t NANOS_PER_MSEC = 1000000ULL;
    double clock2abs = ((double)_timebase_info.denom / (double)_timebase_info.numer) * NANOS_PER_MSEC;
    
    thread_time_constraint_policy_data_t policy;
    policy.period      = 0;
    policy.computation = (uint32_t)(.5* clock2abs); // 5 ms of work
    policy.constraint  = (uint32_t)(1. * clock2abs);
    policy.preemptible = FALSE;
    
    int kr = thread_policy_set(pthread_mach_thread_np(pthread_self()),
                               THREAD_TIME_CONSTRAINT_POLICY,
                               (thread_policy_t)&policy,
                               THREAD_TIME_CONSTRAINT_POLICY_COUNT);
    if (kr != KERN_SUCCESS) {
        mach_error("thread_policy_set:", kr);
        exit(1);
    }
}

static void* _so_hptimer_run(void* param) {
    so_hptimer timer = (so_hptimer) param;
    if (timer) {
        _move_pthread_to_realtime_scheduling_class();
        uint64_t deadline = mach_absolute_time();
        while (timer->mRun) {
            deadline += timer->mTimeInterval;
            mach_wait_until(deadline);
            timer->mAction(timer->mRefCon);
        }
    }
    return NULL;
}

OSStatus so_hptimer_create(so_hptimer* timer,
                           uint64_t timeInterval) {
    if (!timeInterval) {
        return -50;
    }
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        const int retval = mach_timebase_info(&_timebase_info);
        assert(0 == retval && "mach_timebase_info");
    });
    so_hptimer t = *timer = calloc(1, sizeof(so_hptimer_s));
    if (timer) {
        t->mTimeInterval = timeInterval * _timebase_info.denom / _timebase_info.numer;
        return noErr;
    }
    return -1;
}

OSStatus so_hptimer_resume(so_hptimer timer) {
    if (timer && timer->mThread == NULL) {
        timer->mRun = true;
        pthread_create(&timer->mThread, NULL, _so_hptimer_run, timer);
        return noErr;
    }
    return -1;
}

OSStatus so_hptimer_cancel(so_hptimer timer) {
    if (timer) {
        timer->mRun = false;
        return noErr;
    }
    return -50;
}

OSStatus so_hptimer_dispose(so_hptimer timer) {
    if (timer) {
        if (timer->mAction) {
            Block_release(timer->mAction);
        }
        free(timer);
    }
    return noErr;
}

OSStatus so_hptimer_set_action(so_hptimer timer, so_hptimer_action action) {
    if (timer) {
        timer->mAction = Block_copy(action);
        return noErr;
    }
    return -50;
}

OSStatus so_hptimer_set_refcon(so_hptimer timer, const void* refcon) {
    if (timer) {
        timer->mRefCon = (void*) refcon;
        return noErr;
    }
    return -50;
}

