#include "ch.h"
#include "hal.h"
#include "chprintf.h"

BSEMAPHORE_DECL(smph, 0);
//Struct of message to send a data
typedef struct
{
    float humidity;
    float temperature;
    float distance;
} sensor_data_t;

static const uint8_t slave_address = 0x08;
static const uint8_t accel_address = 0x53;

sensor_data_t data;

static WORKING_AREA(waThread_LCD, 128);
static WORKING_AREA(waThread_ACC, 128);

void init_accelerometer(void);

static msg_t Thread_ACC(void *p)
{
    (void)p;
    int x, y, z;
    uint8_t reg = 0x32;
    uint8_t values[6];

    chRegSetThreadName("Read Accelerometer");
    while (TRUE)
    {
        chBSemWait(&smph);
        i2cMasterTransmitTimeout(&I2C0,
                                 accel_address,
                                 &reg, 1,
                                 values,
                                 6,
                                 MS2ST(1000));

        chBSemSignal(&smph);
        chThdSleepMilliseconds(500);

        x = (((int)values[1]) << 8) | values[0];
        y = (((int)values[3]) << 8) | values[2];
        z = (((int)values[5]) << 8) | values[4];

        sdPut(&SD1, (uint8_t)0x7C);
        sdPut(&SD1, (uint8_t)0x18);
        sdPut(&SD1, (uint8_t)0x20);
        chThdSleepMilliseconds(10);

        sdPut(&SD1, (uint8_t)0x7C);
        sdPut(&SD1, (uint8_t)0x19);
        sdPut(&SD1, (uint8_t)0x20);
        chThdSleepMilliseconds(10);

        chprintf((BaseSequentialStream *)&SD1, "x: %d", x);
        chprintf((BaseSequentialStream *)&SD1, "y: %d", y);
        chprintf((BaseSequentialStream *)&SD1, "z: %d", z);
        chThdSleepMilliseconds(2000);
    }
    return 0;
}

static msg_t Thread_LCD(void *p)
{
    (void)p;
    chRegSetThreadName("SerialPrint");
    while (TRUE)
    {
        chBSemWait(&smph);
        i2cMasterTransmitTimeout(&I2C0,
                                 slave_address,
                                 NULL, 0,
                                 (uint8_t *)&data,
                                 sizeof(sensor_data_t),
                                 MS2ST(1000));
        chBSemSignal(&smph);
        chThdSleepMilliseconds(2000);

        sdPut(&SD1, (uint8_t)0x7C);
        sdPut(&SD1, (uint8_t)0x18);
        sdPut(&SD1, (uint8_t)0x20);
        chThdSleepMilliseconds(10);

        sdPut(&SD1, (uint8_t)0x7C);
        sdPut(&SD1, (uint8_t)0x19);
        sdPut(&SD1, (uint8_t)0x20);
        chThdSleepMilliseconds(10);

        chprintf((BaseSequentialStream *)&SD1, "T.: %f", data.temperature);
        chprintf((BaseSequentialStream *)&SD1, "D.: %f", data.distance);
        chprintf((BaseSequentialStream *)&SD1, "H.: %f", data.humidity);
        chThdSleepMilliseconds(2000);
    }
    return 0;
}

/*
 * Application entry point.
 */
int main(void)
{
    halInit();
    chSysInit();

    // Initialize Serial Port
    sdStart(&SD1, NULL);
    chBSemInit(&smph, 0);
    /*
   * I2C initialization.
   */
    I2CConfig i2cConfig;
    i2cStart(&I2C0, &i2cConfig);

    init_accelerometer();

    chThdCreateStatic(waThread_LCD, sizeof(waThread_LCD), NORMALPRIO, Thread_LCD, NULL);
    chThdCreateStatic(waThread_ACC, sizeof(waThread_ACC), NORMALPRIO, Thread_ACC, NULL);

    chThdWait(chThdSelf());

    return 0;
}

void init_accelerometer()
{
    uint8_t msg[2];
    msg[0] = 0x2D;
    msg[1] = 0x01;

    i2cMasterTransmitTimeout(&I2C0,
                             accel_address,
                             msg, 2,
                             NULL,
                             0,
                             MS2ST(1000));
    chThdSleepMilliseconds(50);

    msg[0] = 0x2D;
    msg[1] = 0x08;
    i2cMasterTransmitTimeout(&I2C0,
                             accel_address,
                             msg, 2,
                             NULL,
                             0,
                             MS2ST(1000));
    chThdSleepMilliseconds(50);
}