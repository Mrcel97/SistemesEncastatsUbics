## Activity 2

1. What are the differences between the Blinking example based on Threads and timers? 

- Thread version contain tasks that are scheduled by the operating system.
- In the timer version the functions are executed using OS interruptions.

2. 

**Expected behavior**

The LED connected to the GPIO18 should blink when the 25 GPIO LED blinks.

**Real behavior**

In the RPi the timers are software, confusing the scheduler. 
The timers work correctly, but the task is not working property.

**Modify Thread priority**

We modify the thread priority to HIGHPRIO.
None of the LEDs blink because the OS always schedule the thread task, that is because timers are software in our RPi model.