chaos-clock
===========

Can you make a clock that runs off of chaos theory? Let's find out!

This is an older project of mine, from before I learned such silly things like "test driven development" or "cross-platform functionality" or "not everybody is a physicist". This runs only on windows, takes no input, and writes to "List.txt" possible solutions. Problem is now I only have a linuxbox, so... I'm going to make it platform agnostic, but for now I think it's a cool idea to share.

Imagine a clock that can count only discrete intervals, where the interval is greater than a millisecond. This is actually pretty tricky. A watch might count only seconds, but it counts by measuring an oscillating quartz crystal. With enough time and effort you can get it to measure tens of thousands of seconds. We want something which doesn't "track information" to get the interval - it just falls out.

Enter chaos theory.

Chaotic systems are kinda nutters. Given a position of a chaotic system, it's impossible to tell the past history. But in some cases, if you balance parameters just right, it comes to the same certain position in regular intervals. So we exploit that.

We use a particular type of chaotic system called a "damped-driven pendulum" (http://hyperphysics.phy-astr.gsu.edu/hbase/oscdr.html) and scan over a set of parameters to see if any of them lead to this property. We use an RK4 algorithm to simulate it over a course of 10 hours. Then, if any such positions exist, we dump info on it into a file.

Turns out there are. Woo!

note1: this is actually not perfect evidence, since slight variations in the state of a chaotic system lead to vastly different results. However, it remains a chaotic system, so the proof becomes abstract rather than physical.
note2: yeah, the damped driven pendulum has a clock in it (the driver). DDPs are the easiest chaotic systems to simulate. Still a bit of a cheat.

