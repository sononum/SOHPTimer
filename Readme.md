# SOHPTimer - The sononum high performance timer

This is a repeating timer for iOS and Mac OS. It provides similar functionality like a GCD
timer, but the thread executing the timer action is a _real time_ thread.

I developed this, because of too heavy timer deviations for a project I am developing.

It is highly recommended to read TN2169: https://developer.apple.com/library/ios/technotes/tn2169/_index.html

This are measurments taken on iPhone 4, iOS 7:

2014-06-05 21:26:50.418 SOHPTimer[520:60b] DISPATCH_SOURCE_TYPE_TIMER:
2014-06-05 21:27:10.467 SOHPTimer[520:60b] AVG: 40.00ms	MAX: 65.63ms	MIN: 14.40ms	STD_DEV: 2.13

2014-06-05 21:27:10.471 SOHPTimer[520:60b] so_hptimer:
2014-06-05 21:27:30.557 SOHPTimer[520:60b] AVG: 40.00ms	MAX: 40.15ms	MIN: 39.86ms	STD_DEV: 0.04


The MIT License (MIT)

(c) 2014 Ulrich Zurucker, http://sononum.net

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
