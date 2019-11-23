#include "ch.h"
#include "hal.h"

static void led_clear(void *arg);
static void led_set(void *arg);

static VirtualTimer led_vt_set;
static VirtualTimer led_vt_clear;
static WORKING_AREA(waThread_LED1, 128);

static msg_t Thread_LED1(void *p)
{
  while (1)
  {
    if (chVTIsArmedI(&led_vt_set))
      palSetPad(GPIO18_PORT, GPIO18_PAD);
    else
      palClearPad(GPIO18_PORT, GPIO18_PAD);
  }
}

static void led_clear(void *arg)
{
  palClearPad(GPIO25_PORT, GPIO25_PAD);
  chVTSetI(&led_vt_set, MS2ST(500), led_set, NULL);
}

static void led_set(void *arg)
{
  palSetPad(GPIO25_PORT, GPIO25_PAD);
  chVTSetI(&led_vt_clear, MS2ST(500), led_clear, NULL);
}

void main(void)
{
  halInit();
  chSysInit();

  palSetPadMode(GPIO25_PORT, GPIO25_PAD, PAL_MODE_OUTPUT);
  palSetPadMode(GPIO18_PORT, GPIO18_PAD, PAL_MODE_OUTPUT);

  chThdCreateStatic(waThread_LED1, sizeof(waThread_LED1), NORMALPRIO, Thread_LED1, NULL);
  chVTSetI(&led_vt_set, MS2ST(2000), led_set, NULL);

  chThdWait(chThdSelf());
}