## Activity 3

1. Evaluate the previous code behavior. What happens if the priority of one of the threads is modify to HIGHPRIORITY?, and modifying the CH_CFG_USE_SEMAPHORES_PRIORITY to TRUE?

**High priority thread**

While one LED is blinking the other is waiting, this is because the semaphore. Then one the blocking LED leaves the critical section the other enters and start its cycle. 

**CH_CFG_USE_SEMAPHORES_PRIORITY to TRUE**

The one with with high priority always gets the cpu. Therefore, the other thread never executes.


2. Return the CH_CFG_USE_SEMAPHORES_PRIORITY to FALSE and modify the previous code to use a counting one with counter value N=2, and having both threads NORMAL PRIORITY. Is the behavior different?

The green LED is always on and the red LED blinks according to the counting code. This is because if we set the N = 2 means that 2 threads can enter in the critical zone, making the both LEDs active.


change the priority of one of the threads to HIGHPRIORIY and observe the differences.

The behaviors does not change because we have set the CH_CFG_USE_SEMAPHORES_PRIORITY to FALSE.


