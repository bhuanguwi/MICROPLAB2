# **Versioning** 
Each project file was added and committed on separate branches. When the last project folder (question 3) was pushed onto its own branch, all the branches were merged
into the main branch. The following sections describe which folder and output file correspond to the lab questions. The answers for P1 and P2 are also included at the end.
## **Q1**
Create an example application with three tasks that are created in app_main. Two tasks should share a single GPIO pin (you may use the pin connected to the built-in LED for convenience). One task will turn the GPIO pin on, actively wait for 0.5 seconds, and task-delay for 1 second. The other will turn the GPIO pin off, actively wait for 0.5 seconds, and task-delay for 1 second. The GPIO pin should be managed using a semaphore/mutex. The third task will print a status message only, and task-delay for 1 second. Each task should have it's own priority level. Build and run your code.\

The path **q1/mutexq1** stores the project files for this question. The output file is named **lab2_q1_816022781**

## **Q2**
a) Set the example to round-robin scheduling where tasks have the same priority\
b) Utilise Priority Inheritance to manage the mutex. Vary the assigned priorities for your tasks, and look at the difference in performance. \
Is it what you expected?

The path **q2a/gpio** stores the project files for question 2a. The output file is named **lab2_q2a_816022781**
The paths **q2b/gpio**, **q2b_132** and **q2b_213** stores the project files for question 2b. Each one has different priority levels assigned to the tasks.\
The output files are named **lab2_q2b_816022781**, **lab2_q2b_132_816022781** and **lab2_q2b_213_816022781** respectively.

## **Q3**
Use the IDLE "hook" function to put the processor to sleep for some time when the system is not going to be "in-use". Does the use of the processor \
sleep affect the system performance?\

The path **q3/gpio** stores the project files for question 3. The output file is named **lab2_q3_816022781**

## **P1**
For any project that you have already created, identify the following files and how changes in any one will POTENTIALLY affect the FreeRTOS configuration:
- /project/sdkconfig
- /project/build/include/sdkconfig.h
- /sdk/components/freertos/port/esp8266/include/freertos/FreeRTOSConfig.h

Answer:\
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
- #define configMAX_PRIORITIES 15 - This defines the number of priorities available to the application tasks. In this case, 15 priority levels are available.
- #define configUSE_MUTEXES 1 - This allows mutex functionality to be included in the build which is used for the lab.
- #define configSUPPORT_DYNAMIC_ALLOCATION 1 - This allows the creation of RTOS objects using RAM that is automatically allocated from the FreeRTOS heap. It allows the function
xTaskCreate to be used.
- #define INCLUDE_vTaskDelay 1 - This allows the function vTaskDelay to be used.
- #define INCLUDE_vTaskPrioritySet 1- This allows the function vTaskPrioritySet to be used.
