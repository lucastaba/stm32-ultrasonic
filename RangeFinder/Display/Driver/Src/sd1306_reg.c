#include <stdint.h>

#include "sd1306_reg.h"

#define SD1306_I2C_ADDR_0                                   (0x3C)
#define SD1306_I2C_ADDR_1                                   (0x3D)

#define SD1306_CTRL_BYTE_CMD                                (0x00)
#define SD1306_CTRL_BYTE_DATA                               (0x40)

/* Common Commands */
#define SD1306_CMD_FUNDAMENTAL_SET_CONTRAST                 (0x81)
#define SD1306_CMD_FUNDAMENTAL_SET_DISPLAY_RESUME_TO_RAM    (0xA4)
#define SD1306_CMD_FUNDAMENTAL_SET_DISPLAY_ALL_ON           (0xA5)
#define SD1306_CMD_FUNDAMENTAL_SET_DISPLAY_NORMAL           (0xA6)
#define SD1306_CMD_FUNDAMENTAL_SET_DISPLAY_INVERSE          (0xA7)
#define SD1306_CMD_FUNDAMENTAL_SET_DISPLAY_OFF              (0xAE)
#define SD1306_CMD_FUNDAMENTAL_SET_DISPLAY_ON               (0xAF)

/* Scrolling Commands */
#define SD1306_CMD_SCROLLING_SET_SCROLL_RIGHT               (0x26)
#define SD1306_CMD_SCROLLING_SET_SCROLL_LEFT                (0x27)
#define SD1306_CMD_SCROLLING_SET_SCROLL_UP                  (0x29)
#define SD1306_CMD_SCROLLING_SET_SCROLL_DOWN                (0x2A)
#define SD1306_CMD_SCROLLING_SET_SCROLL_STOP                (0x2E)
#define SD1306_CMD_SCROLLING_SET_SCROLL_START               (0x2F)
#define SD1306_CMD_SCROLLING_SET_SCROLL_VERTICAL_AREA       (0xA3)

/* Addressing Commands */
#define SD1306_CMD_ADDRESSING_SET_MEMORY_ADDR_MODE          (0x20)
#define SD1306_CMD_ADDRESSING_SET_COLUMN_ADDR               (0x21)
#define SD1306_CMD_ADDRESSING_SET_PAGE_ADDR                 (0x22)

/* Hardware Configuration Commands */
#define SD1306_CMD_HW_CONFIG_SET_SEGMENT_REMAP_SEG0_COL0    (0xA0)
#define SD1306_CMD_HW_CONFIG_SET_SEGMENT_REMAP_SEG0_COL127  (0xA1)
#define SD1306_CMD_HW_CONFIG_SET_MULTIPLEX_RATIO            (0xA8)
#define SD1306_CMD_HW_CONFIG_SET_COM_SCAN_DIR_NORMAL        (0xC0)
#define SD1306_CMD_HW_CONFIG_SET_COM_SCAN_DIR_REVERSE       (0xC8)
#define SD1306_CMD_HW_CONFIG_SET_DISPLAY_OFFSET             (0xD3)
#define SD1306_CMD_HW_CONFIG_SET_COM_PIN_CONFIG             (0xDA)

/* Timing & Driving Scheme Commands */
#define SD1306_CMD_TIMING_SET_DISPLAY_CLK_DIVIDE_RATIO_AND_FREQ (0xD5)
#define SD1306_CMD_TIMING_SET_PRECHARGE_PERIOD              (0xD9)
#define SD1306_CMD_TIMING_SET_VCOM_DESELECT_LEVEL           (0xDB)
#define SD1306_CMD_TIMING_SET_NOP                           (0xE3)

/* Scrolling Commands Masks */
#define SD1306_CMD_SCROLLING_SET_SCROLL_PAGE_ADDR_MASK      (0x07)
#define SD1306_CMD_SCROLLING_SET_SCROLL_OFFSET_MASK         (0x3F)
#define SD1306_CMD_SCROLLING_SET_SCROLL_FIXED_AREA_MASK     (0x3F)
#define SD1306_CMD_SCROLLING_SET_SCROLL_AREA_MASK           (0x7F)

/* Addressing Setting Commands Masks */
#define SD1306_CMD_ADDRESSING_SET_LOW_COLUMN_ADDR_MASK      (0x0F)
#define SD1306_CMD_ADDRESSING_SET_HIGH_COLUMN_ADDR_MASK     (0x1F)
#define SD1306_CMD_ADDRESSING_SET_ADDR_MODE_MASK            (0x03)
#define SD1306_CMD_ADDRESSING_SET_COLUMN_ADDR_MASK          (0x3F)
#define SD1306_CMD_ADDRESSING_SET_PAGE_ADDR_MASK            (0x07)
#define SD1306_CMD_ADDRESSING_SET_PAGE_START_ADDR_MASK      (0xB7)

/* Hardware Configuration Commands Masks */
#define SD1306_CMD_HW_CONFIG_SET_DISPLAY_START_LINE_MASK    (0x7F)
#define SD1306_CMD_HW_CONFIG_SET_MULTIPLEX_RATIO_MASK       (0x3F)
#define SD1306_CMD_HW_CONFIG_SET_DISPLAY_OFFSET_MASK        (0x3F)
#define SD1306_CMD_HW_CONFIG_SET_COM_PIN_CONFIG_MASK        (0x03)
#define SD1306_CMD_HW_CONFIG_SET_COM_PIN_CONFIG_SHIFT       (0x04)

/* GDDRAM Page Address */
#define SD1306_GDDRAM_ADDR_PAGE0                            (0x00)
#define SD1306_GDDRAM_ADDR_PAGE1                            (0x01)
#define SD1306_GDDRAM_ADDR_PAGE2                            (0x02)
#define SD1306_GDDRAM_ADDR_PAGE3                            (0x03)
#define SD1306_GDDRAM_ADDR_PAGE4                            (0x04)
#define SD1306_GDDRAM_ADDR_PAGE5                            (0x05)
#define SD1306_GDDRAM_ADDR_PAGE6                            (0x06)
#define SD1306_GDDRAM_ADDR_PAGE7                            (0x07)

#define SD1306_CMD_SUCCESS         (0)
#define SD1306_CMD_I2C_WRITE_ERROR (-1)

static int32_t sd1306_i2c_write_cmd(sd1306_reg_t* obj, const uint8_t* cmd, const uint8_t cmd_len) {
    return obj->i2c_write(obj->obj, cmd, cmd_len);
}

static int32_t sd1306_i2c_read_data(sd1306_reg_t* obj, uint8_t* data, uint8_t data_len) {
    return obj->i2c_read(obj->obj, data, data_len);
}

int32_t sd1306_fundamental_set_contrast(sd1306_reg_t* obj, uint8_t contrast_value) {
    const uint8_t cmd[] = {
        SD1306_CTRL_BYTE_CMD,
        SD1306_CMD_FUNDAMENTAL_SET_CONTRAST,
        contrast_value,
    };
    return sd1306_i2c_write_cmd(obj, cmd, sizeof(cmd)/sizeof(*cmd));
}

int32_t sd1306_fundamental_set_display_resume_to_ram(sd1306_reg_t* obj) {
    const uint8_t cmd[] = {
        SD1306_CTRL_BYTE_CMD,
        SD1306_CMD_FUNDAMENTAL_SET_DISPLAY_RESUME_TO_RAM,
    };
    return sd1306_i2c_write_cmd(obj, cmd, sizeof(cmd)/sizeof(*cmd));
}

int32_t sd1306_fundamental_set_display_all_on(sd1306_reg_t* obj) {
    const uint8_t cmd[] = {
        SD1306_CTRL_BYTE_CMD,
        SD1306_CMD_FUNDAMENTAL_SET_DISPLAY_ALL_ON,
    };
    return sd1306_i2c_write_cmd(obj, cmd, sizeof(cmd)/sizeof(*cmd));
}
int32_t sd1306_fundamental_set_display_normal(sd1306_reg_t* obj) {
    const uint8_t cmd[] = {
        SD1306_CTRL_BYTE_CMD,
        SD1306_CMD_FUNDAMENTAL_SET_DISPLAY_NORMAL,
    };
    return sd1306_i2c_write_cmd(obj, cmd, sizeof(cmd)/sizeof(*cmd));
}

int32_t sd1306_fundamental_set_display_inverse(sd1306_reg_t* obj) {
    const uint8_t cmd[] = {
        SD1306_CTRL_BYTE_CMD,
        SD1306_CMD_FUNDAMENTAL_SET_DISPLAY_INVERSE,
    };
    return sd1306_i2c_write_cmd(obj, cmd, sizeof(cmd)/sizeof(*cmd));
}

int32_t sd1306_fundamental_set_display_off(sd1306_reg_t* obj) {
    const uint8_t cmd[] = {
        SD1306_CTRL_BYTE_CMD,
        SD1306_CMD_FUNDAMENTAL_SET_DISPLAY_OFF,
    };
    return sd1306_i2c_write_cmd(obj, cmd, sizeof(cmd)/sizeof(*cmd));
}

int32_t sd1306_fundamental_set_display_on(sd1306_reg_t* obj) {
    const uint8_t cmd[] = {
        SD1306_CTRL_BYTE_CMD,
        SD1306_CMD_FUNDAMENTAL_SET_DISPLAY_ON,
    };
    return sd1306_i2c_write_cmd(obj, cmd, sizeof(cmd)/sizeof(*cmd));
}

int32_t sd1306_hw_config_set_multiplex_ratio(sd1306_reg_t* obj, const uint8_t multiplex_ratio) {
        const uint8_t cmd[] = {
        SD1306_CTRL_BYTE_CMD,
        SD1306_CMD_HW_CONFIG_SET_MULTIPLEX_RATIO,
        multiplex_ratio,
    };
    return sd1306_i2c_write_cmd(obj, cmd, sizeof(cmd)/sizeof(*cmd));
}

int32_t sd1306_hw_config_set_com_pin_config(sd1306_reg_t* obj, const uint8_t com_pin_config) {
    const uint8_t cmd[] = {
        SD1306_CTRL_BYTE_CMD,
        SD1306_CMD_HW_CONFIG_SET_COM_PIN_CONFIG,
        com_pin_config,
    };
    return sd1306_i2c_write_cmd(obj, cmd, sizeof(cmd)/sizeof(*cmd));
}
