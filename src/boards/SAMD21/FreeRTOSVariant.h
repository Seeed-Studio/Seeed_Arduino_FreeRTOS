#ifndef FREEROTSVARIANT_H
#define FREEROTSVARIANT_H

// Bugfix: Included Arduino.h in extern "C"
#include "task.h"

#ifdef __cplusplus
extern "C" {
#endif
//**************************************************
// defines
//**************************************************

// calibration factor for vNopDelayMS
// used to set accuracy of nopDelayMS function
// this was experimentally chosen from a samd21 processor at 32khz
#define CAL_FACTOR (F_CPU/6000)

//**************************************************
// function prototypes
//**************************************************

// called on fatal error (interrupts disabled already)
void rtosFatalError(void);

// called on full heap or malloc failure
void vApplicationMallocFailedHook(void);

// called on full stack
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName );


// set the error led to use by the rtos
void vSetErrorLed(uint8_t pin, uint8_t activeState);

// blink an error code out the default led when the rtos has crashed
void errorBlink(int errorNumber);

// will delay the processors using nops
// this is used when the rtos has crashed and we cannot use more advanced timing
void vNopDelayMS(unsigned long millis);

#ifdef __cplusplus
}
#endif

#endif
