#ifndef _ARDUINO_FREERTOS_CONFIG_H
#define _ARDUINO_FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

//Load default FreeRTOS configuration
#ifdef ARDUINO_ARCH_SAMD
#if defined(__SAMD51__)
#include "arch/samd51/FreeRTOSConfig.h"
#elif defined(__SAMD21G18A__)
#include "arch/samd21/FreeRTOSConfig.h"
#else
#error "Processor architecture not recognized!"
#endif
#elif defined(ARDUINO_ARCH_STM32)
#if defined(STM32H7xx)
#include "arch/stm32h7xx/FreeRTOSConfig.h"
#else
#error "Processor or architecture not recognized!"
#endif
#else
#error "Processor or architecture not support!"
#endif

// Load extra configurations, which can override the default configuration 
#if __has_include("FreeRTOSConfig_extra.h")
#include "FreeRTOSConfig_extra.h"
#endif
#define configUSE_POSIX_ERRNO 1  
#endif /* _ARDUINO_FREERTOS_CONFIG_H */
