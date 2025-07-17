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
#include "ssd1306_reg.h"

#define CMD_RECEIVED_FLAG (1U << 0U)

typedef struct {
    ARM_DRIVER_I2C* pDrv;
    ssd1306_reg_t reg;
} ssd1306_obj_t;

static void main_thread(void* args);
static void usbd_thread(void* args);
extern int32_t display_driver_parse_and_exec_cmd(ssd1306_obj_t* obj, uint8_t* data, uint16_t len);

int32_t i2c_write(ARM_DRIVER_I2C* pDrv, uint8_t slave_addr, const uint8_t* cmd ,const uint8_t cmd_len);
int32_t i2c_read(ARM_DRIVER_I2C* pDrv, uint8_t slave_addr, uint8_t* data, const uint8_t data_len);
int32_t i2c_write_wrap(void* obj, const uint8_t* cmd ,const uint8_t cmd_len);
int32_t i2c_read_wrap(void* obj, uint8_t* data, const uint8_t data_len);

static ARM_DRIVER_I2C* pI2Cdrv = &Driver_I2C1;
ssd1306_obj_t ssd1306_obj;
osThreadId_t main_thread_id;
osThreadId_t usbd_thread_id;
osEventFlagsId_t usbd_ef;

void main_app(void) {
    volatile osStatus_t status;

    vioInit();
    (void)(pI2Cdrv->Initialize(NULL));
    (void)(pI2Cdrv->PowerControl(ARM_POWER_FULL));
    ssd1306_obj.pDrv = pI2Cdrv;
    ssd1306_obj.reg.i2c_write = i2c_write_wrap;
    ssd1306_obj.reg.i2c_read = i2c_read_wrap;
    ssd1306_obj.reg.obj = &ssd1306_obj;
    status = osKernelInitialize();
    main_thread_id = osThreadNew(main_thread, NULL, NULL);
    usbd_thread_id = osThreadNew(usbd_thread, NULL, NULL);
    usbd_ef = osEventFlagsNew(NULL);
    status = osKernelStart();

    while (1){}

    (void)(pI2Cdrv->PowerControl(ARM_POWER_OFF));
    (void)(pI2Cdrv->Uninitialize());
}

int32_t i2c_write(ARM_DRIVER_I2C* pDrv, uint8_t slave_addr, const uint8_t* cmd ,const uint8_t cmd_len) {
    return pDrv->MasterTransmit((uint32_t)(slave_addr), cmd, (uint32_t)(cmd_len), false);
}

int32_t i2c_read(ARM_DRIVER_I2C* pDrv, uint8_t slave_addr, uint8_t* data, const uint8_t data_len) {
	return 0;
}

int32_t i2c_write_wrap(void* obj, const uint8_t* cmd ,const uint8_t cmd_len) {
    return i2c_write(((ssd1306_obj_t*)obj)->pDrv, 0x3C, cmd, cmd_len);
}

int32_t i2c_read_wrap(void* obj, uint8_t* data, const uint8_t data_len) {
    return i2c_read(((ssd1306_obj_t*)obj)->pDrv, 0x3C, data, data_len);
}

static void main_thread(void* args) {
    while (1) {
        vioSetSignal(vioLED3, vioLEDon);
        osDelay(500U);
        vioSetSignal(vioLED3, vioLEDoff);
        osDelay(500U);
    }
}

static void init_display_default_config(void) {
    volatile int32_t ret;
    osDelay(100U);
    while (pI2Cdrv->GetStatus().busy) {}
    ret = ssd1306_fundamental_set_display_on(&ssd1306_obj.reg, 0x00);
    while (pI2Cdrv->GetStatus().busy) {}
    ret = ssd1306_hw_config_set_multiplex_ratio(&ssd1306_obj.reg, 0x1F);
    while (pI2Cdrv->GetStatus().busy) {}
    ret = ssd1306_hw_config_set_display_offset(&ssd1306_obj.reg, 0x00);
    while (pI2Cdrv->GetStatus().busy) {}
    ret = ssd1306_hw_config_set_display_start_line(&ssd1306_obj.reg, 0x00);
    while (pI2Cdrv->GetStatus().busy) {}
    ret = ssd1306_hw_config_set_segment_remap(&ssd1306_obj.reg, 0x00);
    while (pI2Cdrv->GetStatus().busy) {}
    ret = ssd1306_hw_config_set_com_output_scan_direction(&ssd1306_obj.reg, 0x00);
    while (pI2Cdrv->GetStatus().busy) {}
    ret = ssd1306_hw_config_set_com_pin_config(&ssd1306_obj.reg, 0x00);
    while (pI2Cdrv->GetStatus().busy) {}
    ret = ssd1306_fundamental_set_contrast(&ssd1306_obj.reg, 0x1F);
    while (pI2Cdrv->GetStatus().busy) {}
    ret = ssd1306_fundamental_set_entire_display_on(&ssd1306_obj.reg, 0x01);
    while (pI2Cdrv->GetStatus().busy) {}
    ret = ssd1306_fundamental_set_display_normal_or_inverse(&ssd1306_obj.reg, 0x00);
    while (pI2Cdrv->GetStatus().busy) {}
    ret = ssd1306_timing_and_driving_scheme_set_display_clock_div(&ssd1306_obj.reg, 0xF0);
    while (pI2Cdrv->GetStatus().busy) {}
    ret = ssd1306_charge_pump_set_charge_pump(&ssd1306_obj.reg, 0x01);
    while (pI2Cdrv->GetStatus().busy) {}
    ret = ssd1306_fundamental_set_display_on(&ssd1306_obj.reg, 0x01);
    while (pI2Cdrv->GetStatus().busy) {}
}

extern uint8_t cdc_cmd_buf[256];
extern uint8_t rx_count;
static void usbd_thread(void* args) {
    (void)args;
    const char cmd_prompt_str[] = "Enter display command: ";
    const char cr_lf[] = "\r\n";
    char rsp_buf[256] = {0};
    int32_t ret;

    USBD_Initialize(0U);
    USBD_Connect(0U);
    init_display_default_config();

    for (;;) {
        // USBD_CDC_ACM_WriteData(0, (uint8_t*)(void*)cmd_prompt_str, strlen(cmd_prompt_str));
        (void)osEventFlagsWait(usbd_ef, CMD_RECEIVED_FLAG, osFlagsWaitAny, osWaitForever);
        if ((ret = display_driver_parse_and_exec_cmd(&ssd1306_obj, cdc_cmd_buf, rx_count)) < 0) {
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