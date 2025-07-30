#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "Driver_Common.h"
#include "Driver_I2C.h"
#include "cmsis_vio.h"
#include "cmsis_os2.h"
#include "rl_usb.h"
#include "lvgl.h"

#include "I2C_STM32.h"

#include "main.h"
#include "src/display/lv_display.h"
#include "src/misc/lv_timer.h"
#include "ssd1306.h"

#define SSD1306_I2C_ADDR_0 (0x3C)
#define SSD1306_I2C_ADDR_1 (0x3D)

#define CMD_RECEIVED_FLAG (1U << 0U)
#define MAX_DISPLAY_DATA_LEN (256U)

#define DISPLAY_WIDTH                (128U)
#define DISPLAY_HEIGHT               (32U)
#define DISPLAY_COLOR_DEPTH          (1U)
#define DISPLAY_RENDER_BUFFER_DIVIDE (1U)
#define DISPLAY_RENDER_BUFFER_SIZE   (DISPLAY_WIDTH*DISPLAY_HEIGHT/DISPLAY_RENDER_BUFFER_DIVIDE*DISPLAY_COLOR_DEPTH)

/* External */
extern int32_t display_driver_parse_and_exec_cmd(SSD1306_Object_t* obj, uint8_t* data, uint16_t len);
extern uint8_t cdc_cmd_buf[256];
extern uint8_t rx_count;

/* Local Functions/Prototypes/Variables definition */
static void main_thread(void* args);
static void usbd_thread(void* args);
static void ui_thread(void* args);
static int32_t DisplayWriteI2C(const uint8_t addr, const uint8_t reg, const uint8_t* data, const uint16_t len);
static uint32_t os_get_ticks(void);
static void display_write_data(lv_display_t* display, const lv_area_t* area, uint8_t *px_map);

static ARM_DRIVER_I2C* pI2Cdrv = &Driver_I2C1;
static SSD1306_Object_t display;
static SSD1306_IO_t displayIO;
static osThreadId_t main_thread_id;
static osThreadId_t usbd_thread_id;
static osThreadId_t ui_thread_id;
osEventFlagsId_t usbd_ef;
static uint8_t i2c_data[256] = {0};
static uint8_t render_buffer[DISPLAY_RENDER_BUFFER_SIZE];

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
    display.content = FOLLOW_RAM;
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
    display.phasePeriod1 = 2; /* in DCLOCKs (DCLOCK = clockFreq/clockDiv) */
    display.phasePeriod2 = 2;
    display.deselectLevel = DESELECT_LEVEL_2;
    display.chargePumpEnabled = true;

    (void)SSD1306_RegisterIO(&display, &displayIO);

    /* OS */
    status = osKernelInitialize();
    main_thread_id = osThreadNew(main_thread, NULL, NULL);
    // usbd_thread_id = osThreadNew(usbd_thread, NULL, NULL);
    ui_thread_id = osThreadNew(ui_thread, NULL, NULL);
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
    i2c_data[0] = reg;
    memcpy(i2c_data + 1, data, len);
    return pI2Cdrv->MasterTransmit(addr, i2c_data, len + 1, false);
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

static void ui_thread(void* args) {
    const uint32_t counterDelay = 1000;
    uint32_t counterRemainingTime = counterDelay;
    uint32_t counter = 0;
    uint32_t nextTime;
    uint32_t waitTime;
    lv_display_t* displayUI = NULL;
    lv_obj_t* counterLabel = NULL;
    int i;
    const uint8_t dummyByte = 0;
    volatile int ret;

    (void)SSD1306_Init(&display);
    (void)SSD1306_SetMemoryAddressMode(&display, HORIZONTAL_ADDRESSING_MODE);
    (void)SSD1306_SetColumnAddress(&display, 0, 127);
    (void)SSD1306_SetPageAddressForHVMode(&display, SSD1306_GDDRAM_ADDR_PAGE0, SSD1306_GDDRAM_ADDR_PAGE3);
    for (i = 0; i < 512; i++) {
        ret = SSD1306_GDDRAMwrite(&display, &dummyByte, 1);
    }
    lv_init();
    lv_tick_set_cb(os_get_ticks);
    displayUI = lv_display_create(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    lv_display_set_buffers(displayUI, render_buffer, NULL, DISPLAY_RENDER_BUFFER_SIZE, LV_DISPLAY_RENDER_MODE_FULL);
    lv_display_set_flush_cb(displayUI, display_write_data);
    counterLabel = lv_label_create(lv_screen_active());
    lv_label_set_text(counterLabel, "Hello World");
    lv_obj_align(counterLabel, LV_ALIGN_TOP_LEFT, 0, 0);
    for (;;) {
        nextTime = lv_timer_handler();
        if (nextTime == LV_NO_TIMER_READY) {
            nextTime = LV_DEF_REFR_PERIOD;
        }

        if (nextTime > counterRemainingTime) {
            counter++;
            if (counter > 9) {
                counter = 0;
            }
            waitTime = counterRemainingTime;
            counterRemainingTime = counterDelay;
        } else {
            counterRemainingTime -= nextTime;
            waitTime = nextTime;
        }

        osDelay(waitTime);
    }
}

static uint32_t os_get_ticks(void) {
    return osKernelGetTickCount();
}

static void display_write_data(lv_display_t* displayUI, const lv_area_t* area, uint8_t *px_map) {
    uint8_t pageStart = 0;
    uint8_t pageEnd = 0;
    uint8_t startLine = 0;
    uint16_t dataSize = 0;
    const uint8_t pageHeight = 8;

    pageStart = area->y1 % pageHeight;
    pageEnd = area->y2 % pageHeight;
    startLine = area->x1;
    dataSize = (area->x2 - area->x1 + 1)*(area->y2 - area->y1 + 1);

    SSD1306_GDDRAMwrite(&display, px_map, dataSize);

    lv_display_flush_ready(displayUI);
}

void HAL_Delay(uint32_t Delay) {
    osDelay(Delay);
}