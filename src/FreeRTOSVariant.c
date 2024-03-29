#include <Seeed_Arduino_FreeRTOS.h>
#include <Arduino.h>

extern void freertos_printf(const char *format, ...);

//------------------------------------------------------------------------------
/** delay between led error flashes
 * \param[in] millis milliseconds to delay
 */
static void delayMS(uint32_t millis)
{
  uint32_t iterations = millis * (F_CPU / 7000);
  uint32_t i;
  for (i = 0; i < iterations; ++i)
  {
    __asm__("nop\n\t");
  }
}

//------------------------------------------------------------------------------
/** assertBlink
 * Blink one short pulse every two seconds if configASSERT fails.
*/
void assertBlink()
{
  errorBlink(1);
}
//------------------------------------------------------------------------------
#if (configUSE_MALLOC_FAILED_HOOK == 1)
/** vApplicationMallocFailedHook()
   Blink two short pulses if malloc fails.

	will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
void __attribute__((weak))  vApplicationMallocFailedHook()
{
  errorBlink(2);
}
#endif /* configUSE_MALLOC_FAILED_HOOK == 1 */

//------------------------------------------------------------------------------
#if (configUSE_IDLE_HOOK == 1)
/** vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
void __attribute__((weak)) vApplicationIdleHook(void)
{
  // void loop();
  // loop();
}
#endif /* configUSE_IDLE_HOOK == 1 */

/*-----------------------------------------------------------*/
#if (configCHECK_FOR_STACK_OVERFLOW >= 1)
/**  Blink three short pulses if stack overflow is detected.
	Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected.
  \param[in] pxTask Task handle
  \param[in] pcTaskName Task name
  */
void __attribute__((weak)) vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
  (void)pcTaskName;
  (void)pxTask;
  errorBlink(3);
}
#endif /* configCHECK_FOR_STACK_OVERFLOW >= 1 */

//------------------------------------------------------------------------------
// catch exceptions
/** Hard fault - blink four short flash every two seconds */
void __attribute__((weak))  HardFault_Handler()
{
  errorBlink(4);
}

/** Bus fault - blink five short flashes every two seconds */
void __attribute__((weak))  BusFault_Handler()
{
  errorBlink(5);
}

/** Usage fault - blink six short flashes every two seconds */
void __attribute__((weak))   UsageFault_Handler()
{
  errorBlink(6);
}

/*-----------------------------------------------------------*/
#if (configUSE_TICK_HOOK == 1)
/** This function will be called by each tick interrupt if
	configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
	added here, but the tick hook is called from an interrupt context, so
	code must not attempt to block, and only the interrupt safe FreeRTOS API
	functions can be used (those that end in FromISR()). */
void __attribute__((weak)) vApplicationTickHook()
{
}
#endif /* configUSE_TICK_HOOK == 1 */

/*-----------------------------------------------------------*/
/** Dummy time stats gathering functions need to be defined to keep the
linker happy.  Could edit FreeRTOSConfig.h to remove these.*/
void vMainConfigureTimerForRunTimeStats(void) {}
/** Dummy function
 *  \return zero
 */
unsigned long ulMainGetRunTimeCounterValue() { return 0UL; }

/*
 * override Arduino delay()
 */
extern void _real_delay(unsigned long ms);
void delay(unsigned long ms)
{
  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
  {
    vTaskDelay(ms / portTICK_PERIOD_MS);
  }
  else
  {
    _real_delay(ms);
  }
}

void yield(void)
{
  if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING)
  {
    taskYIELD();
  }
}

#ifdef ARDUINO_ARCH_SAMD
#if defined(__SAMD51__)
#include "arch/samd51/FreeRTOSVariant.h"
#elif defined(__SAMD21G18A__)
#include "arch/samd21/FreeRTOSVariant.h"
#else
#error "Processor architecture not recognized!"
#endif
#elif defined(ARDUINO_ARCH_STM32)
#if defined(STM32H7xx)
#include "arch/stm32h7xx/FreeRTOSVariant.h"
#else
#error "Processor or architecture not recognized!"
#endif
#else
#error "Processor or architecture not support!"
#endif