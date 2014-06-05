//
//  HTViewController.m
//  SOHPTimer
//
//  Created by Ulrich Zurucker on 05.06.14.
//  Copyright (c) 2014 sononum.net. All rights reserved.
//

#import "HTViewController.h"
#import <mach/mach_time.h>
#import "so_hptimer.h"

static uint64_t interval_ms = 1;
static unsigned num_measurements = 100;

@interface HTViewController () {
    dispatch_source_t           mTimer;
    struct mach_timebase_info   _timebase_info;
    uint64_t                    _lastTime;
    unsigned                    _numMeasurements;
    uint64_t*                   _deltas;
    so_hptimer                  mHPTimer;
}


@end

@implementation HTViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    NSLog(@"DISPATCH_SOURCE_TYPE_TIMER:");
    _deltas = malloc(sizeof(uint64_t) * num_measurements);
    mach_timebase_info(&_timebase_info);
    mTimer = dispatch_source_create(DISPATCH_SOURCE_TYPE_TIMER, 0, 0, dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0));
    
    dispatch_semaphore_t s = dispatch_semaphore_create(0);
    dispatch_source_set_event_handler(mTimer, ^{
        const uint64_t t = mach_absolute_time() *_timebase_info.numer / _timebase_info.denom;
        if (_lastTime) {
            const uint64_t delta_ns = t - _lastTime;
            _deltas[_numMeasurements] = delta_ns;
            if (_numMeasurements++ == num_measurements) {
                dispatch_semaphore_signal(s);
            }
        }
        _lastTime = t;
    });
    dispatch_source_set_timer(mTimer,
                              DISPATCH_TIME_NOW,
                              interval_ms * NSEC_PER_MSEC,
                              0);
    dispatch_resume(mTimer);
    dispatch_semaphore_wait(s, DISPATCH_TIME_FOREVER);
    
    dispatch_suspend(mTimer);
    [self calculateStatistics];

    NSLog(@"so_hptimer:");
    _lastTime = _numMeasurements = 0;

    so_hptimer_create(&mHPTimer, interval_ms * NSEC_PER_MSEC);
    so_hptimer_set_maxrepetitions(mHPTimer, num_measurements);
    so_hptimer_set_action(mHPTimer, ^(void *refcon) {
        const uint64_t t = mach_absolute_time() *_timebase_info.numer / _timebase_info.denom;
        if (_lastTime) {
            const uint64_t delta_ns = t - _lastTime;
            _deltas[_numMeasurements] = delta_ns;
        }
        _lastTime = t;
    });
    so_hptimer_resume(mHPTimer);
    so_hptimer_waituntilfinished(mHPTimer);
    [self calculateStatistics];
}

- (void)calculateStatistics {
    double avg = .0;
    for (int i=0; i<num_measurements; ++i) {
        avg += _deltas[i];
    }
    avg /= num_measurements;

    double a = .0;
    double max = -MAXFLOAT;
    double min = MAXFLOAT;
    for (int i=0; i<num_measurements; ++i) {
        a += pow((_deltas[i] - avg), 2.);
        max = MAX(_deltas[i], max);
        min = MIN(_deltas[i], min);
    }
    const double std_dev = sqrt(a / num_measurements);
    NSLog(@"AVG: %.2fms\tMAX: %.2fms\tMIN: %.2fms\tSTD_DEV: %.2f", avg * 1e-6, max * 1e-6, min * 1e-6, std_dev * 1e-6);
}

@end
