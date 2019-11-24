## Activity 1

2. Modify the Priority of the new Thread to HIGHPRIO. When running against hs changed the behavior in any way? Why?

There's no difference because when the threads executes the `chThdSleepMilliseconds` they free the CPU, therfore the other thread starts the execution.

3. Exchange the code in the new Thread with the code present in the boc maintaining HIGHPRIO. Observe the behavior differences and explain the reason for them.

Only the second LED blinks because sleeping using `whiles` consumes CPU. Then when the second thread consumes his quantum return to the processes queue at the first position due to its higher priority, making the other thread (with lower priority) to never have access to the CPU.

4. Is there any difference if the Thread returns to NORMALPRIO?

It happens the same as the 2nd question. As the both threads have the same priority they share the CPU (when the second thread consumes the quantum it goes back to the queue and the first threat goes to the CPU).

5. Modify the CH_TIME_QUANTUM present in the `chconf.h` file to 0 and observe again the differences. What’s the reason?

CH_TIME_QUANTUM == 0 -> Cooperative (Until the thread frees the CPU the other won't start)

Similar case to the 3th answer because the second led always consumes CPU and never frees it.

6. Assign HIGHPRIO to the first Thread and NORMALPRIO to the second one and comment the behavior.

In Cooperative mode is a thread has a higher priority the scheduler frees the CPU so lower priority threads can enter to it.



