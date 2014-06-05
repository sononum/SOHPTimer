//
//  SOHPTimerTests.m
//  SOHPTimerTests
//
//  Created by Ulrich Zurucker on 05.06.14.
//  Copyright (c) 2014 sononum.net. All rights reserved.
//

#import <XCTest/XCTest.h>
#import "so_hptimer.h"

@interface SOHPTimerTests : XCTestCase

@end

@implementation SOHPTimerTests

- (void)testTimer
{
    so_hptimer timer;
    XCTAssertEqual(noErr, so_hptimer_create(&timer, 100 * NSEC_PER_MSEC), @"must create timer");
    so_hptimer_set_action(timer, ^() {
        NSLog(@"hi!");
    });
    XCTAssertEqual(noErr, so_hptimer_resume(timer), @"must resume timer");
    sleep(1);
    XCTAssertEqual(noErr, so_hptimer_suspend(timer), @"must suspend timer");
    XCTAssertEqual(noErr, so_hptimer_resume(timer), @"must resume timer");
    sleep(1);
    XCTAssertEqual(noErr, so_hptimer_suspend(timer), @"must cancel timer");
    XCTAssertEqual(noErr, so_hptimer_dispose(timer), @"must dispose timer");
}

- (void)testTimerRepetitions1 {
    const uint64_t num_expected = 42;
    __block uint64_t count = 0;

    so_hptimer timer;
    XCTAssertEqual(noErr, so_hptimer_create(&timer, 100 * NSEC_PER_MSEC), @"must create timer");
    so_hptimer_set_action(timer, ^{
        ++count;
    });
    XCTAssertEqual(noErr, so_hptimer_set_maxrepetitions(timer, num_expected), @"must set num reps");
    XCTAssertEqual(noErr, so_hptimer_resume(timer), @"must resume timer");
    XCTAssertEqual(noErr, so_hptimer_waituntilfinished(timer), @"must wait until finished");
    XCTAssertEqual(num_expected, count, @"must have invoked block");
    XCTAssertEqual(noErr, so_hptimer_dispose(timer), @"must dispose timer");
}

@end
