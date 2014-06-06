# SOHPTimer - The sononum high precision timer

This is a repeating timer for iOS and Mac OS. It provides similar functionality like a GCD
timer, but the thread executing the timer action is a _real time_ thread.

I developed this, because of too heavy timer deviations for a project I am developing.

It is highly recommended to read TN2169: https://developer.apple.com/library/ios/technotes/tn2169/_index.html

*usage:*
Just add so_hptimer.c/h to your project. The repo contains an entire Xcode project with a little test app, 
that does measurements with a DISPATCH_SOURCE_TYPE_TIMER and a so_hptimer.

This are measurments taken on iPhone 4, iOS 7:

DISPATCH_SOURCE_TYPE_TIMER:
AVG: 40.00ms	MAX: 65.63ms	MIN: 14.40ms	STD_DEV: 2.13

so_hptimer:
AVG: 40.00ms	MAX: 40.15ms	MIN: 39.86ms	STD_DEV: 0.04


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
