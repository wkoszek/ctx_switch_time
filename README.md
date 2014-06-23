ctxswtch_time
=============

ctxswtch_time - tries to show the characteristics of thread scheduling

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
