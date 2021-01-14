//**************************************************************************
// FreeRtos on Samd21
// By Scott Briscoe
//
// Project is a simple example of how to get FreeRtos running on a SamD21 processor
// Project can be used as a template to build your projects off of as well
//**************************************************************************

#include <Seeed_Arduino_FreeRTOS.h>
#include "task.h"
//**************************************************************************
// Type Defines and Constants
//**************************************************************************

#define  ERROR_LED_PIN  13 //Led Pin: Typical Arduino Board


#define ERROR_LED_LIGHTUP_STATE  LOW // the state that makes the led light up on your board, either low or high

// Select the serial port the project should use and communicate over
// Sombe boards use SerialUSB, some use Serial
#define LL          SerialUSB
//#define LL          Serial

//**************************************************************************
// global variables
//**************************************************************************
TaskHandle_t Handle_aTask;
TaskHandle_t Handle_bTask;
TaskHandle_t Handle_monitorTask;
//*****************************************************************
// Create a thread that prints out A to the screen every two seconds
// this task will delete its self after printing out afew messages
//*****************************************************************
static void threadA(void* pvParameters) {

    LL.println("Thread A: Started");
    for (int x = 0; x < 20; ++x) {
        LL.print("A");
        delay(500);
    }

    // delete ourselves.
    // Have to call this or the system crashes when you reach the end bracket and then get scheduled.
    LL.println("Thread A: Deleting");
    vTaskDelete(NULL);
}

//*****************************************************************
// Create a thread that prints out B to the screen every second
// this task will run forever
//*****************************************************************
static void threadB(void* pvParameters) {
    LL.println("Thread B: Started");

    while (1) {
        LL.println("B");
        delay(2000);
    }

}

//*****************************************************************
// Task will periodicallt print out useful information about the tasks running
// Is a useful tool to help figure out stack sizes being used
//*****************************************************************
void taskMonitor(void* pvParameters) {
    int x;
    int measurement;

    LL.println("Task Monitor: Started");

    // run this task afew times before exiting forever
    for (x = 0; x < 10; ++x) {

        LL.println("");
        LL.println("******************************");
        LL.println("[Stacks Free Bytes Remaining] ");

        measurement = uxTaskGetStackHighWaterMark(Handle_aTask);
        LL.print("Thread A: ");
        LL.println(measurement);

        measurement = uxTaskGetStackHighWaterMark(Handle_bTask);
        LL.print("Thread B: ");
        LL.println(measurement);

        measurement = uxTaskGetStackHighWaterMark(Handle_monitorTask);
        LL.print("Monitor Stack: ");
        LL.println(measurement);

        LL.println("******************************");

        delay(10000); // print every 10 seconds
    }

    // delete ourselves.
    // Have to call this or the system crashes when you reach the end bracket and then get scheduled.
    LL.println("Task Monitor: Deleting");
    vTaskDelete(NULL);

}


//*****************************************************************

void setup() {

    LL.begin(115200);
    while (!LL) ;  // Wait for serial terminal to open port before starting program
    delay(1000); // prevents usb driver crash on startup, do not omit this

    LL.println("");
    LL.println("******************************");
    LL.println("        Program start         ");
    LL.println("******************************");

    // Create the threads that will be managed by the rtos
    // Sets the stack size and priority of each task
    // Also initializes a handler pointer to each task, which are important to communicate with and retrieve info from tasks
    xTaskCreate(threadA,     "Task A",       256, NULL, tskIDLE_PRIORITY + 3, &Handle_aTask);
    xTaskCreate(threadB,     "Task B",       256, NULL, tskIDLE_PRIORITY + 2, &Handle_bTask);
    xTaskCreate(taskMonitor, "Task Monitor", 256, NULL, tskIDLE_PRIORITY + 1, &Handle_monitorTask);

    // Start the RTOS, this function will never return and will schedule the tasks.
    vTaskStartScheduler();

    while(1){};

}

//*****************************************************************
// loop() never run at this demo
//*****************************************************************
void loop() {
   
}


//*****************************************************************