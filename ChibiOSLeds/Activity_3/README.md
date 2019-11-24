## Activity 3

1. Evaluate the previous code behavior. What happens if the priority of one of the threads is modify to HIGHPRIORITY?, and modifying the CH_CFG_USE_SEMAPHORES_PRIORITY to TRUE?

**High priority thread**

While one LED is blinking the other is waiting, this is because the semaphore. Then one the blocking LED leaves the critical section the other enters and start its cycle. 

**CH_CFG_USE_SEMAPHORES_PRIORITY to TRUE**



2. Return the CH_CFG_USE_SEMAPHORES_PRIORITY to FALSE and modify the previous code to use a counting one with counter value N=2, and having both threads NORMAL PRIORITY. Is the behavior different? , change the priority of one of the threads to HIGHPRIORIY and observe the differences.
