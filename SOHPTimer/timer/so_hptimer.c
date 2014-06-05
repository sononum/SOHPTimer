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

static const double _default_workamount = .05;

static struct mach_timebase_info _timebase_info;

typedef struct so_hptimer_s {
    uint64_t            mTimeInterval;
    uint64_t            mMaxRepetitions;
    uint64_t            mNumRepetitions;
    so_hptimer_action   mAction;
    pthread_t           mThread;
    double              mWorkAmount;
} so_hptimer_s;

static void _move_pthread_to_realtime_scheduling_class(const so_hptimer timer) {
    thread_time_constraint_policy_data_t policy;
    policy.period      = 0;
    policy.computation = (uint32_t)(timer->mWorkAmount * timer->mTimeInterval);
    policy.constraint  = 2 * policy.computation;
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
        _move_pthread_to_realtime_scheduling_class(timer);
        uint64_t deadline = mach_absolute_time();
        while (timer->mNumRepetitions--) {
            deadline += timer->mTimeInterval;
            mach_wait_until(deadline);
            timer->mAction();
        }
    }
    return NULL;
}

OSStatus so_hptimer_create(so_hptimer* timer,
                           uint64_t timeInterval) {
    if (!timeInterval) {
        return -50;
    }
    so_hptimer t = *timer = calloc(1, sizeof(so_hptimer_s));
    if (timer) {
        so_hptimer_set_maxrepetitions(t, UINT64_MAX);
        return so_hptimer_set_interval(t, timeInterval);
    }
    return -1;
}

OSStatus so_hptimer_set_interval(so_hptimer timer,
                                 uint64_t timeInterval) {
    if (timer) {
        if (!timeInterval) {
            return -50;
        }
        static dispatch_once_t onceToken;
        dispatch_once(&onceToken, ^{
            const int retval = mach_timebase_info(&_timebase_info);
            assert(0 == retval && "mach_timebase_info");
        });
        timer->mWorkAmount = _default_workamount;
        timer->mTimeInterval = timeInterval * _timebase_info.denom / _timebase_info.numer;
        return noErr;
    }
    return -1;
}

OSStatus so_hptimer_set_workamount(so_hptimer timer, double workAmount) {
    if (timer && !timer->mNumRepetitions && isnormal(workAmount) && workAmount > .0 && workAmount < 1.0) {
        timer->mWorkAmount = workAmount;
        return noErr;
    }
    return -50;
}

OSStatus so_hptimer_get_interval(const so_hptimer timer,
                                 uint64_t* outTimeInterval) {
    if (timer && outTimeInterval) {
        if (timer->mTimeInterval) {
            *outTimeInterval = timer->mTimeInterval * _timebase_info.numer / _timebase_info.denom;
        } else {
            return -50;
        }
    }
    return -1;
}

OSStatus so_hptimer_set_maxrepetitions(so_hptimer timer,
                                       uint64_t maxRepetitions) {
    if (timer) {
        timer->mMaxRepetitions = maxRepetitions;
        return noErr;
    }
    return -50;
}

OSStatus so_hptimer_waituntilfinished(const so_hptimer timer) {
    if (timer) {
        if (NULL == timer->mThread) {
            return -50;
        }
        pthread_join(timer->mThread, NULL);
        timer->mThread = NULL;
        return noErr;
    }
    return -1;
}

OSStatus so_hptimer_resume(so_hptimer timer) {
    if (timer) {
        timer->mNumRepetitions = timer->mMaxRepetitions;
        if (timer->mThread == NULL) {
            return pthread_create(&timer->mThread, NULL, _so_hptimer_run, timer);
        }
        return noErr;
    }
    return -1;
}

OSStatus so_hptimer_suspend(so_hptimer timer) {
    if (timer) {
        timer->mNumRepetitions = 0;
        return so_hptimer_waituntilfinished(timer);
    }
    return -50;
}

OSStatus so_hptimer_dispose(so_hptimer timer) {
    if (timer) {
        if (timer->mNumRepetitions) {
            so_hptimer_resume(timer);
        }
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
