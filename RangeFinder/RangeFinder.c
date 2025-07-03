#include <stdint.h>
#include <stdbool.h>

#include "Driver_Common.h"
#include "Driver_I2C.h"
#include "cmsis_vio.h"

#include "I2C_STM32.h"

#include "main.h"
#include "sd1306_reg.h"

typedef struct {
    ARM_DRIVER_I2C* pDrv;
    sd1306_reg_t reg;
} sd1306_obj_t;

static void delay(uint32_t ms);
int32_t i2c_write(ARM_DRIVER_I2C* pDrv, uint8_t slave_addr, const uint8_t* cmd ,const uint8_t cmd_len);
int32_t i2c_read(ARM_DRIVER_I2C* pDrv, uint8_t slave_addr, uint8_t* data, const uint8_t data_len);
int32_t i2c_write_wrap(void* obj, const uint8_t* cmd ,const uint8_t cmd_len);
int32_t i2c_read_wrap(void* obj, uint8_t* data, const uint8_t data_len);

static ARM_DRIVER_I2C* pI2Cdrv = &Driver_I2C1;
sd1306_obj_t sd1306_obj;

void main_app(void) {
    int32_t ret;
    vioInit();
    (void)(pI2Cdrv->Initialize(NULL));
    (void)(pI2Cdrv->PowerControl(ARM_POWER_FULL));
    sd1306_obj.pDrv = pI2Cdrv;
    sd1306_obj.reg.i2c_write = i2c_write_wrap;
    sd1306_obj.reg.i2c_read = i2c_read_wrap;
    sd1306_obj.reg.obj = &sd1306_obj;
    ret = sd1306_hw_config_set_multiplex_ratio(&sd1306_obj.reg, 0xFF);
    ret = sd1306_hw_config_set_com_pin_config(&sd1306_obj.reg, 0x02);
    ret = sd1306_fundamental_set_contrast(&sd1306_obj.reg, 0x8F);
    ret = sd1306_fundamental_set_display_all_on(&sd1306_obj.reg);
    while (1) {
        vioSetSignal(vioLED3, vioLEDon);
        ret = sd1306_fundamental_set_display_on(&sd1306_obj.reg);
        delay(500U);
        vioSetSignal(vioLED3, vioLEDoff);
        ret = sd1306_fundamental_set_display_off(&sd1306_obj.reg);
        delay(500U);
    }
    (void)(pI2Cdrv->PowerControl(ARM_POWER_OFF));
    (void)(pI2Cdrv->Uninitialize());
}

static void delay(uint32_t ms) {
    uint32_t delayTime;

    if (ms > ((UINT32_MAX / 100000U) * 4U)) {
        return;
    }

    delayTime = ms * 100000U / 4U;
    while (delayTime--) {
        __NOP();
    }
}

int32_t i2c_write(ARM_DRIVER_I2C* pDrv, uint8_t slave_addr, const uint8_t* cmd ,const uint8_t cmd_len) {
    int32_t ret;
    if ((ret = pDrv->MasterTransmit((uint32_t)(slave_addr), cmd, (uint32_t)(cmd_len), false)) == ARM_DRIVER_OK) {
        while ((ret = pDrv->GetDataCount()) > 0 && ret != cmd_len) {}
    }
    return ret;
}

int32_t i2c_read(ARM_DRIVER_I2C* pDrv, uint8_t slave_addr, uint8_t* data, const uint8_t data_len) {
    return 0;
}

int32_t i2c_write_wrap(void* obj, const uint8_t* cmd ,const uint8_t cmd_len) {
    return i2c_write(((sd1306_obj_t*)obj)->pDrv, 0x3C, cmd, cmd_len);
}

int32_t i2c_read_wrap(void* obj, uint8_t* data, const uint8_t data_len) {
    return i2c_read(((sd1306_obj_t*)obj)->pDrv, 0x3C, data, data_len);
}
