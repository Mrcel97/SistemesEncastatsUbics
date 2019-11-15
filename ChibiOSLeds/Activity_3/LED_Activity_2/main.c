#include "ch.h"
#include "hal.h"

BSEMAPHORE_DECL(smph, 0);

static WORKING_AREA(waThread_LED1, 128);
static msg_t Thread_LED1(void *p)
{
  (void)p;
  chRegSetThreadName("blinker-1");
  while (TRUE)
  {
    chBSemWait(&smph);
    for (int count = 0; count < 5; count++)
    {
      palSetPad(GPIO25_PORT, GPIO25_PAD);
      chThdSleepMilliseconds(500);
      palClearPad(GPIO25_PORT, GPIO25_PAD);
      chThdSleepMilliseconds(500);
    }
    chBSemSignal(&smph);
  }
  return 0;
}

static WORKING_AREA(waThread_LED2, 128);
static msg_t Thread_LED2(void *p)
{
  (void)p;
  chRegSetThreadName("blinker-2");
  while (TRUE)
  {
    chBSemWait(&smph);
    palSetPad(GPIO18_PORT, GPIO18_PAD);
    for (int count = 0; count < 5; count++)
    {
      palSetPad(GPIO18_PORT, GPIO18_PAD);
      chThdSleepMilliseconds(100);
      palClearPad(GPIO18_PORT, GPIO18_PAD);
      chThdSleepMilliseconds(300);
    }
    palClearPad(GPIO18_PORT, GPIO18_PAD);
    chBSemSignal(&smph);
  }
  return 0;
}

int main()
{
  halInit();
  chSysInit();

  palSetPadMode(GPIO25_PORT, GPIO25_PAD, PAL_MODE_OUTPUT);
  palSetPadMode(GPIO18_PORT, GPIO18_PAD, PAL_MODE_OUTPUT);

  chBSemInit(&smph, 0);

  chThdCreateStatic(waThread_LED1, sizeof(waThread_LED1), HIGHPRIO, Thread_LED1, NULL);
  chThdCreateStatic(waThread_LED2, sizeof(waThread_LED2), NORMALPRIO, Thread_LED2, NULL);

  chThdWait(chThdSelf());
}