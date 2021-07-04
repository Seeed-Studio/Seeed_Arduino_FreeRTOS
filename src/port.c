#include <Arduino.h>

#ifndef __CORTEX_M
#pragma GCC error  "no \"__CORTEX_M\" definition"
#endif

#if (__CORTEX_M == 0x00U)
#include "../FreeRTOS/Source/portable/GCC/ARM_CM0/port.c"
#elif (__CORTEX_M == 0x03U)
#include "../FreeRTOS/Source/portable/GCC/ARM_CM3/port.c"
#elif (__CORTEX_M == 0x04U)
#include "../FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c"
#elif (__CORTEX_M == 0x07U)
#include "../FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.c"
#endif

