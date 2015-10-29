ctx_switch_time -- Context Switch Time Inference Program
=============

[![Build Status](https://travis-ci.org/wkoszek/ctx_switch_time.svg)](https://travis-ci.org/wkoszek/ctx_switch_time)

ctx_switch_time - tries to show the characteristics of thread scheduling

The principle is pretty trivial--you preserve larger chunk of memory.
You keep spinning in the loop and reading the real-time clock readouts.
Somewhere in the process of doing so you'll be rescheduled, and once your
thread will be picked by the OS once again, the next readout will be
much different.

By analyzing the time samples, one can figure out when the context switch 
happened.

Some values for tuning:
- small
- bfactor
- big_dev

# Author

- Wojciech Adam Koszek, [wojciech@koszek.com](mailto:wojciech@koszek.com)
- [http://www.koszek.com](http://www.koszek.com)
