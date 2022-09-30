#if defined(ARDUINO_ARCH_SAMD) && defined(__SAMD51__)

#include <Arduino.h>
#include <FreeRTOS.h>
#include "FreeRTOSVariant.h"

#ifdef __cplusplus
extern "C"
{
#endif

  // this is referring to the loop function of your arduino project
  extern void loop(void);

  //************************************************************************
  // global variables

  int ErrorLed_Pin = LED_BUILTIN; //default arduino led pin
  int ErrorLed_ActiveState = LOW;

  //************************************************************************

  // called on fatal error (interrupts disabled already)
  void rtosFatalError(void)
  {

    Serial1.println("Fatal Error");

    while (1)
    {
      errorBlink(3);
    }
  }

  // called on full heap or malloc failure
  void vApplicationMallocFailedHook(void)
  {

    Serial1.println("Malloc Failed");

    while (1)
    {
      errorBlink(1);
    }
  }

  // called on full stack
  void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
  {
    (void)xTask;
    Serial1.print("Stack Overflow: ");
    Serial1.println(pcTaskName);

    while (1)
    {
      errorBlink(2);
    }
  }

  /* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  /* If the buffers to be provided to the Idle task are declared inside this
   * function then they must be declared static - otherwise they will be allocated on
   * the stack and so not exists after this function exits. */
  static StaticTask_t xIdleTaskTCB;
  static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

  /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
    state will be stored. */
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

  /* Pass out the array that will be used as the Idle task's stack. */
  *ppxIdleTaskStackBuffer = uxIdleTaskStack;

  /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer, StackType_t **ppxTimerTaskStackBuffer, uint32_t *pulTimerTaskStackSize )
{
  /* If the buffers to be provided to the Timer task are declared inside this
   * function then they must be declared static - otherwise they will be allocated on
   * the stack and so not exists after this function exits. */
  static StaticTask_t xTimerTaskTCB;
  static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

  /* Pass out a pointer to the StaticTask_t structure in which the Timer
    task's state will be stored. */
  *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

  /* Pass out the array that will be used as the Timer task's stack. */
  *ppxTimerTaskStackBuffer = uxTimerTaskStack;

  /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}


  //************************************************************************

  // set the error led to use by the rtos
  void vSetErrorLed(uint8_t pin, uint8_t activeState)
  {
    ErrorLed_Pin = pin;
    ErrorLed_ActiveState = activeState;
  }

  // blink an error code out the default led when the rtos has crashed
  void errorBlink(int errorNumber)
  {
    pinMode(ErrorLed_Pin, OUTPUT);

    for (int x = 0; x < errorNumber; ++x)
    {
      digitalWrite(ErrorLed_Pin, ErrorLed_ActiveState);
      vNopDelayMS(100);
      digitalWrite(ErrorLed_Pin, !ErrorLed_ActiveState);
      vNopDelayMS(100);
    }

    vNopDelayMS(1000);
  }

  // will delay the processors using nops
  // this is used when the rtos has crashed and we cannot use more advanced timing
  void vNopDelayMS(unsigned long millis)
  {
    unsigned long iterations = millis * CAL_FACTOR;
    unsigned long i;
    for (i = 0; i < iterations; ++i)
    {
      asm volatile("nop\n\t");
    }
  }

  /*
 * override Arduino delay()
 */
  extern void _real_delay(unsigned long ms);
  void delay(unsigned long ms)
  {
    if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
    {
      vTaskDelay((ms * 1000) / portTICK_PERIOD_US);
    }
    else
    {
      _real_delay(ms);
    }
  }

  void __attribute__((weak)) vApplicationIdleHook(void)
  {
    /*
   * calling loop() here is a bug,
   * which may cause loop() be called firstly,
   * before calling of setup().
  loop(); //will use your projects loop function as the rtos idle loop
   */
  tud_cdc_write_flush();
  }

  void yield(void)
  {
    if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING)
    {
      taskYIELD();
    }
    tud_cdc_write_flush();
  }

#ifdef __cplusplus
}
#endif

#if defined(USE_TINYUSB)
void tinyusb_task_backgroud(void *arg)
{
  (void *)arg;
  // tinyusb wait for device to be configured (in setup())
  delay(1000);
  Adafruit_TinyUSB_Core_init();
  while (1)
  {
    tud_task();
    tud_cdc_write_flush();
  }
  vTaskDelete(NULL);
}
void tinyusb_task()
{
  xTaskCreate(tinyusb_task_backgroud, "tub back", 512, NULL, configMAX_PRIORITIES - 2, NULL);
}
#endif

#endif