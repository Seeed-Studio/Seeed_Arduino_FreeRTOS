#if defined(USE_TINYUSB)
void tinyusb_task_backgroud(void *arg)
{
  (void *)arg;
  while (1)
  {
    TinyUSB_Device_Task();
    TinyUSB_Device_FlushCDC();
    delay(10);
  }
  vTaskDelete(NULL);
}
void tinyusb_task()
{
  xTaskCreate(tinyusb_task_backgroud, "tub back", 256, NULL, configMAX_PRIORITIES, NULL);
}
#endif

extern void xPortSysTickHandler(void);
int sysTickHook(void)
{
  if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING)
  {
    xPortSysTickHandler();
  }
  return 0; // return zero to keep running the arduino default handler!
}