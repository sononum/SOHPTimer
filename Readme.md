# SOHPTimer - The sononum high performance timer

This is a repeating timer for iOS and Mac OS. It provides similar functionality like a GCD
timer, but the thread executing the timer action is a _real time_ thread.

I developed this, because of too heavy timer deviations for a project I am developing.

This are measurments taken on iPhone 4, iOS 7:

2014-06-05 21:26:50.418 SOHPTimer[520:60b] DISPATCH_SOURCE_TYPE_TIMER:
2014-06-05 21:27:10.467 SOHPTimer[520:60b] AVG: 40.00ms	MAX: 65.63ms	MIN: 14.40ms	STD_DEV: 2.13

2014-06-05 21:27:10.471 SOHPTimer[520:60b] so_hptimer:
2014-06-05 21:27:30.557 SOHPTimer[520:60b] AVG: 40.00ms	MAX: 40.15ms	MIN: 39.86ms	STD_DEV: 0.04

(c) 2014 Ulrich Zurucker, http://sononum.net

LICENSE: BSD License
