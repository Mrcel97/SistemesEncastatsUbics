#include "ch.h"
#include "hal.h"

SEMAPHORE_DECL(smph, 2);

static WORKING_AREA(waThread_LED1, 128);
static msg_t Thread_LED1(void *p)
{
  (void)p;
  chRegSetThreadName("blinker-1");
  while (TRUE)
  {
    chSemWait(&smph);
    for (int cont = 0; cont < 5; cont++)
    {
      palSetPad(GPIO25_PORT, GPIO25_PAD);
      chThdSleepMilliseconds(500);
      palClearPad(GPIO25_PORT, GPIO25_PAD);
      chThdSleepMilliseconds(500);
    }
    chSemSignal(&smph);
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
    chSemWait(&smph);
    palSetPad(GPIO18_PORT, GPIO18_PAD);
    for (int cont = 0; cont < 5; cont++)
    {
      palSetPad(GPIO25_PORT, GPIO25_PAD);
      chThdSleepMilliseconds(100);
      palClearPad(GPIO25_PORT, GPIO25_PAD);
      chThdSleepMilliseconds(300);
    }
    palClearPad(GPIO18_PORT, GPIO18_PAD);
    chSemSignal(&smph);
  }
  return 0;
}

int main(void)
{
  halInit();
  chSysInit();

  palSetPadMode(GPIO25_PORT, GPIO25_PAD, PAL_MODE_OUTPUT);
  palSetPadMode(GPIO18_PORT, GPIO18_PAD, PAL_MODE_OUTPUT);

  chSemInit(&smph, 2);

  chThdCreateStatic(waThread_LED1, sizeof(waThread_LED1), HIGHPRIO, Thread_LED1, NULL);
  chThdCreateStatic(waThread_LED2, sizeof(waThread_LED2), NORMALPRIO, Thread_LED2, NULL);

  chThdWait(chThdSelf());

  return 0;
}
