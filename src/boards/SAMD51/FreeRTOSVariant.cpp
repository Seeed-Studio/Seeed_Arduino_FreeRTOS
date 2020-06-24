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

  int ErrorLed_Pin = 13; //default arduino led pin
  int ErrorLed_ActiveState = LOW;

  //************************************************************************

  // called on fatal error (interrupts disabled already)
  void rtosFatalError(void)
  {

    Serial.println("Fatal Error");

    while (1)
    {
      errorBlink(3);
    }
  }

  // called on full heap or malloc failure
  void vApplicationMallocFailedHook(void)
  {

    Serial.println("Malloc Failed");

    while (1)
    {
      errorBlink(1);
    }
  }

  // called on full stack
  void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
  {

    Serial.print("Stack Overflow: ");
    Serial.println(pcTaskName);

    while (1)
    {
      errorBlink(2);
    }
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
  }

  void yield(void)
  {
    if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING)
    {
      taskYIELD();
    }
  }

#ifdef __cplusplus
}
#endif

#if defined(USE_TINYUSB)
void tinyusb_task_backgroud(void *arg)
{
  (void *)arg;
  while (1)
  {
    tud_task();
    tud_cdc_write_flush();
    delay(10);
  }
  vTaskDelete(NULL);
}
void tinyusb_task()
{
  xTaskCreate(tinyusb_task_backgroud, "tub back", 256, NULL, configMAX_PRIORITIES, NULL);
}
#endif

#endif