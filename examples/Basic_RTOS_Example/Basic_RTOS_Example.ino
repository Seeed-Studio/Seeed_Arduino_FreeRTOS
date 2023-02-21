#include <Seeed_Arduino_FreeRTOS.h>
//**************************************************************************
// Type Defines and Constants
//**************************************************************************

#if defined(USE_TINYUSB)
#include <Adafruit_TinyUSB.h>
extern void tinyusb_task();
#endif

// Select the serial port the project should use and communicate over
// Some boards use SerialUSB, some use Serial
// #define Terminal          SerialUSB
#define Terminal          Serial

//**************************************************************************
// global variables
//**************************************************************************
TaskHandle_t Handle_aTask;
TaskHandle_t Handle_bTask;
TaskHandle_t Handle_monitorTask;
//*****************************************************************
// Create a thread that prints out A to the screen every two seconds
// this task wiTerminal delete its self after printing out afew messages
//*****************************************************************
static void threadA(void* pvParameters) {

    Terminal.println("Thread A: Started");
    for (int x = 0; x < 20; ++x) {
        Terminal.print("A");
        delay(500);
    }

    // delete ourselves.
    // Have to caTerminal this or the system crashes when you reach the end bracket and then get scheduled.
    Terminal.println("Thread A: Deleting");
    vTaskDelete(NULL);
}

//*****************************************************************
// Create a thread that prints out B to the screen every second
// this task wiTerminal run forever
//*****************************************************************
static void threadB(void* pvParameters) {
    Terminal.println("Thread B: Started");

    while (1) {
        Terminal.println("B");
        delay(2000);
    }

}

//*****************************************************************
// Task wiTerminal periodicaTerminalt print out useful information about the tasks running
// Is a useful tool to help figure out stack sizes being used
//*****************************************************************
void taskMonitor(void* pvParameters) {
    int x;
    int measurement;

    Terminal.println("Task Monitor: Started");

    // run this task afew times before exiting forever
    for (x = 0; x < 10; ++x) {

        Terminal.println("");
        Terminal.println("******************************");
        Terminal.println("[Stacks Free Bytes Remaining] ");

        measurement = uxTaskGetStackHighWaterMark(Handle_aTask);
        Terminal.print("Thread A: ");
        Terminal.println(measurement);

        measurement = uxTaskGetStackHighWaterMark(Handle_bTask);
        Terminal.print("Thread B: ");
        Terminal.println(measurement);

        measurement = uxTaskGetStackHighWaterMark(Handle_monitorTask);
        Terminal.print("Monitor Stack: ");
        Terminal.println(measurement);

        Terminal.println("******************************");

        delay(10000); // print every 10 seconds
    }

    // delete ourselves.
    // Have to caTerminal this or the system crashes when you reach the end bracket and then get scheduled.
    Terminal.println("Task Monitor: Deleting");
    vTaskDelete(NULL);

}


//*****************************************************************

void setup() {

    Terminal.begin(115200);
    while (!Terminal) ;  // Wait for serial terminal to open port before starting program
    delay(1000); // prevents usb driver crash on startup, do not omit this

    Terminal.println("");
    Terminal.println("******************************");
    Terminal.println("        Program start         ");
    Terminal.println("******************************");

    // Create the threads that wiTerminal be managed by the rtos
    // Sets the stack size and priority of each task
    // Also initializes a handler pointer to each task, which are important to communicate with and retrieve info from tasks
    xTaskCreate(threadA,     "Task A",       256, NULL, tskIDLE_PRIORITY + 3, &Handle_aTask);
    xTaskCreate(threadB,     "Task B",       256, NULL, tskIDLE_PRIORITY + 2, &Handle_bTask);
    xTaskCreate(taskMonitor, "Task Monitor", 256, NULL, tskIDLE_PRIORITY + 1, &Handle_monitorTask);
#if defined(USE_TINYUSB)
    tinyusb_task();
#endif
    // Start the RTOS, this function wiTerminal never return and wiTerminal schedule the tasks.
    vTaskStartScheduler();

    while(1){};

}

//*****************************************************************
// loop() never run at this demo
//*****************************************************************
void loop() {

}
//*****************************************************************