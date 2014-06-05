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

typedef void (^so_hptimer_action)(void*);

OSStatus so_hptimer_create(so_hptimer* timer,
                           uint64_t timeInterval);

OSStatus so_hptimer_resume(so_hptimer timer);

OSStatus so_hptimer_cancel(so_hptimer timer);

OSStatus so_hptimer_dispose(so_hptimer timer);

OSStatus so_hptimer_set_action(so_hptimer timer, so_hptimer_action action);

OSStatus so_hptimer_set_refcon(so_hptimer timer, const void* refcon);

#endif
