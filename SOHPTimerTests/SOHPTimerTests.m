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

- (void)setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown
{
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testTimer
{
    so_hptimer timer;
    XCTAssertEqual(noErr, so_hptimer_create(&timer, 1000 * NSEC_PER_MSEC), @"must create timer");
    so_hptimer_set_action(timer, ^(void * refcon) {
        NSLog(@"hi!");
    });
    XCTAssertEqual(noErr, so_hptimer_resume(timer), @"must resume timer");
    sleep(10);
    XCTAssertEqual(noErr, so_hptimer_cancel(timer), @"must cancel timer");
    XCTAssertEqual(noErr, so_hptimer_dispose(timer), @"must dispose timer");
}

@end
