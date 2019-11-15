#include "ch.h"
#include "hal.h"

static void led_clear(void *arg);
static void led_set(void *arg);

static VirtualTimer led_vt1;

static void led_clear(void *arg){
  palClearPad(GPIO25_PORT, GPIO25_PAD);
  chVTSetI(&led_vt1, MS2ST(500), led_set, NULL);
}

static void led_set(void *arg){
  palSetPad(GPIO25_PORT, GPIO25_PAD);
  chVTSetI(&led_vt1, MS2ST(500), led_clear, NULL);
}

void main(void)
{
  halInit();
  chSysInit();

  palSetPadMode(GPIO25_PORT, GPIO25_PAD, PAL_MODE_OUTPUT);

  chVTSetI(&led_vt1, MS2ST(2000), led_set, NULL);

  chThdWait(chThdSelf());
}