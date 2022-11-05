# **STUDENT ID - 816022781**
## **Versioning** 
Each project file was added and committed on separate branches. When the last project folder (question 3) was pushed onto its own branch, all the branches were merged
into the main branch. The following sections describe which folder and output file correspond to the lab questions. The answers for P1, P2, 2b and 3 are also included at the end.
## **Q1**
Create an example application with three tasks that are created in app_main. Two tasks should share a single GPIO pin (you may use the pin connected to the built-in LED for convenience). One task will turn the GPIO pin on, actively wait for 0.5 seconds, and task-delay for 1 second. The other will turn the GPIO pin off, actively wait for 0.5 seconds, and task-delay for 1 second. The GPIO pin should be managed using a semaphore/mutex. The third task will print a status message only, and task-delay for 1 second. Each task should have it's own priority level. Build and run your code.

The path **q1/mutexq1** stores the project files for this question. The output file is named **lab2_q1_816022781**

## **Q2**
a) Set the example to round-robin scheduling where tasks have the same priority\
b) Utilise Priority Inheritance to manage the mutex. Vary the assigned priorities for your tasks, and look at the difference in performance.
Is it what you expected?

The path **q2a/gpio** stores the project files for question 2a. The output file is named **lab2_q2a_816022781**
The paths **q2b/gpio**, **q2b_132** and **q2b_213** stores the project files for question 2b. Each one has different priority levels assigned to the tasks.\
The output files are named **lab2_q2b_816022781**, **lab2_q2b_132_816022781** and **lab2_q2b_213_816022781** respectively.

## **Q3**
Use the IDLE "hook" function to put the processor to sleep for some time when the system is not going to be "in-use". Does the use of the processor 
sleep affect the system performance?

The path **q3/gpio** stores the project files for question 3. The output file is named **lab2_q3_816022781**

## **P1**
For any project that you have already created, identify the following files and how changes in any one will POTENTIALLY affect the FreeRTOS configuration:
- /project/sdkconfig
- /project/build/include/sdkconfig.h
- /sdk/components/freertos/port/esp8266/include/freertos/FreeRTOSConfig.h

Answer:
1. The SDK configuration file contains options that enable or disable SDK modules. Using question 2 from lab 1 as the reference, if changes are made to the sdkconfig file, 
then the application source code may not compile properly or the application may not respond as expected. The FreeRTOS configuration would be unaffected by these changes.
For example, in the sdkconfig file you can set the baud rate in the line "CONFIG_ESPTOOLPY_BAUD=115200". If this parameter is changed, it can affect communication from between the ESP chip and the PC.

2. The sdkconfig.h file is the header file which is created based off the parameters in the sdkconfig file. This is where you would find the global definitions for various parameters such as the baud rate, the target and more.
Hence, if changes made in this file, it wouldn't affect the FreeRTOS configuration.

3. FreeRTOS is customized using a configuration file called FreeRTOSconfig.h. It tailors the RTOS kernel to the application
being built. Hence, if changes were made to this file, then it might affect how FreeRTOS processes data and events. For example,
in the FreeRTOSconfig.h file you can enable preemptive scheduling by defining configUSE_PREEMPTION as 0 or 1. This would affect how tasks are scheduled.

## **P2**
Identify the labels that must be #define'd and the functions that provide task performance information in the ESP SDK.

Some of the functions that provide task performance information are as follows:
- vTaskStartScheduler - starts the RTOS scheduler.
- xTaskCreate - Create a new task and add it to the list of tasks that are ready to run.
- vTaskDelay - Delay a task or block the task for a given number of ticks
- vTaskPrioritySet - Sets the priority or any task.

Some of the #define'd labels for task performance information are as follows:
- #define configUSE_PREEMPTION 1 - This enables the use of preemptive scheduling in the RTOS scheduler.
- #define configUSE_TIME_SLICING 1 - This enables time slicing for round robin scheduling.
- #define configUSE_IDLE_HOOK 1 - This enables the idle hook function to run during the idle task.
- #define configMAX_PRIORITIES 15 - This defines the number of priorities available to the application tasks. In this case, 15 priority levels are available.
- #define configUSE_MUTEXES 1 - This allows mutex functionality to be included in the build which is used for the lab.
- #define configSUPPORT_DYNAMIC_ALLOCATION 1 - This allows the creation of RTOS objects using RAM that is automatically allocated from the FreeRTOS heap. It allows the function
xTaskCreate to be used.
- #define INCLUDE_vTaskDelay 1 - This allows the function vTaskDelay to be used.
- #define INCLUDE_vTaskPrioritySet 1- This allows the function vTaskPrioritySet to be used.

## **Discussion for 2a** 

Round robin scheduling was used where each task had the same priority level. Time slicing was also enabled which was configured to be 1 tick (equivalent to 1 ms in our case). Hence the tasks were assigned a time slot of 1 ms. As seen in the output file, Task 1 ran but then Task 2 and 3 tries to run right after but could not because task 1 still has the mutex key. This also causes the active wait of 0.5 seconds in task 1 to be a lot longer due to round-robin scheduling because it has to attempt to run task 2 and 3. The active wait ended up waiting approximately 1 second which is seen in line 146 in the output file. Eventually, task 3 gets to run next and task 2 runs right after. The wait time for task 2 was 0.5 seconds because all other tasks were still blocked as seen in line 151-152 in the output file. The performance was as expected and aligned with the expectations of a round-robin scheduler.

## **Discussion for 2b** 
Task 1 was for turning the LED on and actively waiting 0.5 seconds then task delaying 1 second.
Task 2 was for turning the LED off and actively waiting 0.5 seconds then task delaying 1 second.
Task 3 was for a status update on the pin state then task delaying 1 second.

1. Project file q2b assigned priority levels as follows:\
Task 1 - 3\
Task 2 - 2\
Task 3 - 1

2. Project file q2b_132 assigned priority levels as follows:\
Task 1 - 1\
Task 2 - 3\
Task 3 - 2

3. Project file q2b_213 assigned priority levels as follows:\
Task 1 - 2\
Task 2 - 1\
Task 3 - 3

With reference to "q2b", task 1 ran first, actively waited for 0.5 seconds, then it was task delayed for 1 second. Similarly task 2 ran next , actively waited for 0.5 seconds and was task delayed for one second. Lastly, task 3 ran last and task delays for one second. It then idles for 0.5 seconds until the sequence repeats again. This was expected as the highest priority task always ran before the lower priroity task.

However in "q2b_132", we can see priority inheritance taking place as there is an instance where the lower priority task runs before the higher priority one. Initially, task 2 runs first, actively waits for 0.5 seconds and then task delays for one second. Then the lower priority task, "Task 1" becomes ready and runs next. "Task 3" is also becomes ready while Task 1 is already running. Task 3 does not get to run because it does not have the key to the mutex which is why the output keeps sending the message "Task 3 : Semaphore was not obtained". This shows priority inheritance taking place due to the usage of the mutex which allows the lower priority task to finish before the higher priority task can run. As Task 1 was finished, Task 3 ran next.

Similarly in "q2b_213" , in the output files, we can see that in line 54-55, there is priority inheritance occuring as the lower priority task, "Task 1" is ready before the higher priority task, "Task 3" therefore it runs first. As task 1 is running, task 3 is prevented from running because it cannot obtain the mutex key. This is indicated by the message on the output file which says "Task 3 : Semaphore was not obtained". This was expected since priority inheritance is implemented with the usage of a mutex which allows lower priority tasks to block a higher priority task from running.

## **Discussion for 3**

The code and task priority levels from "q2b" were copied for "q3". An idle hook function was added to the file so that the system can enter a low power state when it is idle. The idle hook function calls the portSUPPRESS_TICKS_AND_SLEEP() function specific to FreeRTOS which puts the system into the low power state until a task is ready. As described previously, the tasks 1,2,3 ran sequentially in the order stated where there was 0.5 seconds idle time after task 3 completed. This is seen in line 38-39 in the output file for q2b. Now with an idle hook function that puts the system into low power state, we can see that this idle hook gets called from line 39 to line 87 for 0.5 seconds which it was idle for. As a task becomes ready, the idle hook/task stops running and the task sequence repeats. The idle hook did not affect system performance with regards to task scheduling as the system outputs were exactly the same as "q2b". However, it helps conserve power whenever the system is idle.
