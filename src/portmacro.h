#ifndef _PORTMACRO_H_
#define _PORTMACRO_H_
#ifdef ARDUINO_ARCH_SAMD
#include "sam.h"
#endif

#ifdef ARDUINO_ARCH_STM32
#include "stm32_def.h"
#endif

#ifndef __CORTEX_M
#pragma GCC error  "no \"__CORTEX_M\" definition"
#endif

#if (__CORTEX_M == 0x00U)
#include "../FreeRTOS/Source/portable/GCC/ARM_CM0/portmacro.h"
#elif (__CORTEX_M == 0x03U)
#include "../FreeRTOS/Source/portable/GCC/ARM_CM3/portmacro.h"
#elif (__CORTEX_M == 0x04U)
#include "../FreeRTOS/Source/portable/GCC/ARM_CM4F/portmacro.h"
#elif (__CORTEX_M == 0x07U)
#include "../FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/portmacro.h"
#endif
#endif //_PORTMACRO_H_
