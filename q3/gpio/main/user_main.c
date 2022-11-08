
/*
Use priority inheritance with mutex
#define configUSE_PREEMPTION 1, to enable preemptive scheduling
#define configUSE_TIME_SLICING      0,  disable time slicing
*/

/*
To implement idle hook
#define configUSE_IDLE_HOOK to 1 in FreeRTOSConfig.h
#define configUSE_TICKLESS_IDLE 1 or 2 to use default and user defined portSUPPRESS_TICKS_AND_SLEEP( xExpectedIdleTime ) respectively

Create user defined function or use the default provided portSUPPRESS_TICKS_AND_SLEEP( xExpectedIdleTime )
Define an idle hook function that has the following name and prototype:
void vApplicationIdleHook( void ) that displays a message showing that the system is idle
portSUPPRESS_TICKS_AND_SLEEP( xExpectedIdleTime ) is automatically called when idle which puts system into low power mode.
*/

/* OTHER CHANGES
changed from time to wait for mutex from 1 ms to 10 ms so less debug messages are output on putty
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
// Include semaphore header file 
#include "freertos/semphr.h"

#include "driver/gpio.h"

#include "esp_log.h"
#include "esp_system.h"

static const char* TAG = "main";

#define configUSE_IDLE_HOOK 1
#define configUSE_TICKLESS_IDLE 1
#define configUSE_TIME_SLICING 0
#define GPIO_OUTPUT_IO_0    2
#define GPIO_OUTPUT_PIN_SEL (1ULL<<GPIO_OUTPUT_IO_0) 
#define TASK1_PRIORITY		3
#define TASK2_PRIORITY		2
#define TASK3_PRIORITY		1
#define PRINTSTATS_PRIORITY	4 // Highest priority to ensure stats are printed
//HALF_SECOND_DELAY was calculated to be 2650000
#define HALF_SECOND_DELAY	2650000

SemaphoreHandle_t xSemaphore = NULL;
unsigned int delay_value;
static void Task1(void * pvParameters);
static void Task2(void * pvParameters);
static void Task3(void * pvParameters);
static void PrintStats( void *pvParameters );
void vApplicationIdleHook (void);
//void portSUPPRESS_TICKS_AND_SLEEP(TickType_t xExpectedIdleTime);

// Code to print task performance information from Mastering the FreeRTOS Real Time Kernel
// A Hands on Tutorial Guide on page 348
static void PrintStats( void *pvParameters )
{
TickType_t xLastExecutionTime;
/* The buffer used to hold the formatted run-time statistics text needs to be quite
large. It is therefore declared static to ensure it is not allocated on the task
stack. This makes this function non re-entrant. */
static char cStringBuffer[ 512 ];
/* The task will run every 5 seconds. */
const TickType_t xBlockPeriod = pdMS_TO_TICKS( 5000 );
 /* Initialize xLastExecutionTime to the current time. This is the only time this
 variable needs to be written to explicitly. Afterwards it is updated internally
 within the vTaskDelayUntil() API function. */
 xLastExecutionTime = xTaskGetTickCount();
 /* As per most tasks, this task is implemented in an infinite loop. */
 for( ;; )
 {
 /* Wait until it is time to run this task again. */
 vTaskDelayUntil( &xLastExecutionTime, xBlockPeriod );
 /* Generate a text table from the run-time stats. This must fit into the
 cStringBuffer array. */
 vTaskGetRunTimeStats( cStringBuffer );

 /* Print out column headings for the run-time stats table. */
 printf( "\nTask\t\tAbs\t\t%%\n" );
 printf( "-------------------------------------------------------------\n" );

 /* Print out the run-time stats themselves. The table of data contains
 multiple lines, so the vPrintMultipleLines() function is called instead of
 calling printf() directly. vPrintMultipleLines() simply calls printf() on
 each line individually, to ensure the line buffering works as expected. */
 printf("%.*s",512,cStringBuffer);
 }
}

/* A task that uses the semaphore. */
static void Task1(void * pvParameters)
{
	
	/* ... Do other things. */
	while (1) {
		if (xSemaphore != NULL)
		{
			
			/* See if we can obtain the semaphore.  If the semaphore is not
			available wait 10 ms to see if it becomes free. */
			if (xSemaphoreTake(xSemaphore, (TickType_t) 10/portTICK_RATE_MS ) == pdTRUE)
			{
				/* We were able to obtain the semaphore and can now access the
				shared resource. */

				/* ... */
				gpio_set_level(GPIO_OUTPUT_IO_0, 0);
				ESP_LOGI(TAG, "GPIO2 pin set to %d, LED ON", gpio_get_level(GPIO_OUTPUT_IO_0));
				// active wait for 0.5 seconds
				while (delay_value != HALF_SECOND_DELAY) {
					delay_value++;
				}
				ESP_LOGI(TAG, "Task 1 Completed");
				/* We have finished accessing the shared resource.  Release the
				semaphore. */
				delay_value = 0;
				xSemaphoreGive(xSemaphore);
				vTaskDelay(1000 / portTICK_RATE_MS);
			}
			else
			{
				ESP_LOGI(TAG, "Task 1 : Semaphore was not obtained");
				/* We could not obtain the semaphore and can therefore not access
				the shared resource safely. */
			}
		}
	}
}

/* A task that uses the semaphore. */
static void Task2(void * pvParameters)
{
	
	/* ... Do other things. */
	while (1) {
		if (xSemaphore != NULL)
		{
			/* See if we can obtain the semaphore.  If the semaphore is not
			available wait 10 ms to see if it becomes free. */
			if (xSemaphoreTake(xSemaphore, (TickType_t) 10/portTICK_RATE_MS ) == pdTRUE)
			{
				/* We were able to obtain the semaphore and can now access the
				shared resource. */

				/* ... */
				gpio_set_level(GPIO_OUTPUT_IO_0, 1);
				ESP_LOGI(TAG, "GPIO2 pin set to %d, LED OFF", gpio_get_level(GPIO_OUTPUT_IO_0));
				while (delay_value != HALF_SECOND_DELAY) {
					delay_value++;
				}
				ESP_LOGI(TAG, "Task 2 Completed");
				/* We have finished accessing the shared resource.  Release the
				semaphore. */
				delay_value = 0;
				xSemaphoreGive(xSemaphore);
				vTaskDelay(1000 / portTICK_RATE_MS);
			}
			else
			{
				ESP_LOGI(TAG, "Task 2 : Semaphore was not obtained");
				/* We could not obtain the semaphore and can therefore not access
				the shared resource safely. */
			}
		}
	}
}

/* A task that uses the semaphore. */
static void Task3(void * pvParameters)
{
	/* ... Do other things. */
	while (1) {
		if (xSemaphore != NULL)
		{
			/* See if we can obtain the semaphore.  If the semaphore is not
			available wait 10 ms to see if it becomes free. */
			if (xSemaphoreTake(xSemaphore, (TickType_t) 10/portTICK_RATE_MS ) == pdTRUE)
			{
				/* We were able to obtain the semaphore and can now access the
				shared resource. */

				/* ... */
				ESP_LOGI(TAG, "Status Update, Pin State : %d", gpio_get_level(GPIO_OUTPUT_IO_0));
				ESP_LOGI(TAG, "Task 3 Completed");
				/* We have finished accessing the shared resource.  Release the
				semaphore. */
				xSemaphoreGive(xSemaphore);
				vTaskDelay(1000 / portTICK_RATE_MS);
			}
			else
			{
				ESP_LOGI(TAG, "Task 3 : Semaphore was not obtained");
				/* We could not obtain the semaphore and can therefore not access
				the shared resource safely. */
			}
		}
	}
}

// idle hook function 
void vApplicationIdleHook (void)
{
	ESP_LOGI(TAG, "System is IDLE, In Low Power Mode");

}

// Task scheduler in Freertos is automatically started in app_main , therefore it not necessary to start it manually
void app_main(void)
{
  gpio_config_t io_conf;
  //disable interrupt
  io_conf.intr_type = GPIO_INTR_DISABLE;
  //set as output mode
  io_conf.mode = GPIO_MODE_OUTPUT;
  //bit mask of the pins that you want to set,e.g.GP2
  io_conf.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
  //disable pull-down mode
  io_conf.pull_down_en = 0;
  //disable pull-up mode
  io_conf.pull_up_en = 0;
  //configure GPIO with the given settings
  gpio_config(&io_conf);


  delay_value = 0;
  TaskHandle_t xTask1, xTask2, xTask3, xPrintStats;
  xSemaphore = xSemaphoreCreateMutex();
  

xTaskCreate(Task1, "LED On", 4096, NULL, TASK1_PRIORITY, &xTask1);
xTaskCreate(Task2, "LED OFF", 4096, NULL, TASK2_PRIORITY, &xTask2);
xTaskCreate(Task3, "Status", 4096, NULL, TASK3_PRIORITY, &xTask3);
xTaskCreate(PrintStats, "Task Stats", 4096, NULL,PRINTSTATS_PRIORITY, &xPrintStats );

//vTaskStartScheduler();
//ESP_LOGI(TAG, "Task Scheduler initialized");

}




