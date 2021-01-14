extern void xPortSysTickHandler(void);
int osSystickHandler(void)
{
  if (xTaskGetSchedulerState() == taskSCHEDULER_RUNNING)
  {
    xPortSysTickHandler();
  }
  return 0; // return zero to keep running the arduino default handler!
}