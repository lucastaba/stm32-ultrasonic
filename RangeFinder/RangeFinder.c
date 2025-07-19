#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "Driver_Common.h"
#include "Driver_I2C.h"
#include "cmsis_vio.h"
#include "cmsis_os2.h"
#include "rl_usb.h"

#include "I2C_STM32.h"

#include "main.h"
#include "ssd1306.h"

#define SSD1306_I2C_ADDR_0 (0x3C)
#define SSD1306_I2C_ADDR_1 (0x3D)

#define CMD_RECEIVED_FLAG (1U << 0U)
#define MAX_DISPLAY_DATA_LEN (256U)

/* External */
extern int32_t display_driver_parse_and_exec_cmd(SSD1306_Object_t* obj, uint8_t* data, uint16_t len);
extern uint8_t cdc_cmd_buf[256];
extern uint8_t rx_count;

/* Local Functions/Prototypes/Variables definition */
static void main_thread(void* args);
static void usbd_thread(void* args);
static int32_t DisplayWriteI2C(const uint8_t addr, const uint8_t reg, const uint8_t* data, const uint16_t len);

static ARM_DRIVER_I2C* pI2Cdrv = &Driver_I2C1;
static SSD1306_Object_t display;
static SSD1306_IO_t displayIO;
static osThreadId_t main_thread_id;
static osThreadId_t usbd_thread_id;
osEventFlagsId_t usbd_ef;
static uint8_t i2c_data[256] = {0};

/* Implementation */
void main_app(void) {
    volatile osStatus_t status;

    /* Board */
    vioInit();
    (void)(pI2Cdrv->Initialize(NULL));
    (void)(pI2Cdrv->PowerControl(ARM_POWER_FULL));
    
    /* Diplay */
    displayIO.Init = NULL;
    displayIO.DeInit = NULL;
    displayIO.i2cAddress = SSD1306_I2C_ADDR_0;
    displayIO.WriteReg = DisplayWriteI2C;
    displayIO.ReadReg = NULL;

    display.contrast = 255;
    display.content = ALL_ON;
    display.color = NORMAL;
    display.displayState = ON;
    display.startLine = 0;
    display.segmentMap = SEG0_TO_0;
    display.multiplexRatio = 63; /* HEIGHT - 1 */
    display.scanDirection = COM0_TO_N;
    display.displayOffset = 0;
    display.pinSequence = SEQUENTIAL;
    display.pinDirection = RIGHT_TO_LEFT;
    display.clockFreq = 0xF;
    display.clockDiv = 1;
    display.phasePeriod1 = 1; /* in DCLOCKs (DCLOCK = clockFreq/clockDiv) */
    display.phasePeriod2 = 1;
    display.deselectLevel = DESELECT_LEVEL_1;

    (void)SSD1306_RegisterIO(&display, &displayIO);

    /* OS */
    status = osKernelInitialize();
    main_thread_id = osThreadNew(main_thread, NULL, NULL);
    usbd_thread_id = osThreadNew(usbd_thread, NULL, NULL);
    usbd_ef = osEventFlagsNew(NULL);
    status = osKernelStart();

    while (1){}

    (void)(pI2Cdrv->PowerControl(ARM_POWER_OFF));
    (void)(pI2Cdrv->Uninitialize());
}

static int32_t DisplayWriteI2C(const uint8_t addr, const uint8_t reg, const uint8_t* data, const uint16_t len) {
    static uint8_t displayData[MAX_DISPLAY_DATA_LEN];
    
    if (len > MAX_DISPLAY_DATA_LEN) {
        return -1;
    }

    while (pI2Cdrv->GetStatus().busy) {}
    memcpy(displayData, data, len);
    return pI2Cdrv->MasterTransmit(addr, displayData, len, false);
}

static void main_thread(void* args) {
    while (1) {
        vioSetSignal(vioLED3, vioLEDon);
        osDelay(500U);
        vioSetSignal(vioLED3, vioLEDoff);
        osDelay(500U);
    }
}


static void usbd_thread(void* args) {
    (void)args;
    char rsp_buf[256] = {0};
    int32_t ret;

    USBD_Initialize(0U);
    USBD_Connect(0U);
    SSD1306_Init(&display);

    for (;;) {
        (void)osEventFlagsWait(usbd_ef, CMD_RECEIVED_FLAG, osFlagsWaitAny, osWaitForever);
        if ((ret = display_driver_parse_and_exec_cmd(&display, cdc_cmd_buf, rx_count)) < 0) {
            memset(rsp_buf, 0, sizeof(rsp_buf));
            snprintf(rsp_buf, sizeof(rsp_buf), "Command failed with error: %d\r\n", ret);
            USBD_CDC_ACM_WriteData(0, (uint8_t*)(void*)rsp_buf, strlen(rsp_buf));
        }
        while (pI2Cdrv->GetStatus().busy) {}
    }
}

void HAL_Delay(uint32_t Delay) {
    osDelay(Delay);
}