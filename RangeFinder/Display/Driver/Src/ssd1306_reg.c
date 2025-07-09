#include <stdint.h>

#include "ssd1306_reg.h"

#define SSD1306_I2C_ADDR_0                                   (0x3C)
#define SSD1306_I2C_ADDR_1                                   (0x3D)

#define SSD1306_CTRL_BYTE_CMD                                (0x00)
#define SSD1306_CTRL_BYTE_DATA                               (0x40)

/* Common Commands */
#define SSD1306_CMD_FUNDAMENTAL_SET_CONTRAST                 (0x81)
#define SSD1306_CMD_FUNDAMENTAL_SET_ENTIRE_DISPLAY_ON        (0xA4)
#define SSD1306_CMD_FUNDAMENTAL_SET_DISPLAY_NORMAL_OR_INVERSE (0xA6)
#define SSD1306_CMD_FUNDAMENTAL_SET_DISPLAY_ON               (0xAE)

/* Scrolling Commands */
#define SSD1306_CMD_SCROLLING_SET_SCROLL_RIGHT               (0x26)
#define SSD1306_CMD_SCROLLING_SET_SCROLL_LEFT                (0x27)
#define SSD1306_CMD_SCROLLING_SET_SCROLL_UP                  (0x29)
#define SSD1306_CMD_SCROLLING_SET_SCROLL_DOWN                (0x2A)
#define SSD1306_CMD_SCROLLING_SET_SCROLL_STOP                (0x2E)
#define SSD1306_CMD_SCROLLING_SET_SCROLL_START               (0x2F)
#define SSD1306_CMD_SCROLLING_SET_SCROLL_VERTICAL_AREA       (0xA3)

/* Addressing Commands */
#define SSD1306_CMD_ADDRESSING_SET_MEMORY_ADDR_MODE          (0x20)
#define SSD1306_CMD_ADDRESSING_SET_COLUMN_ADDR               (0x21)
#define SSD1306_CMD_ADDRESSING_SET_PAGE_ADDR                 (0x22)

/* Hardware Configuration Commands */
#define SSD1306_CMD_HW_CONFIG_SET_DISPLAY_START_LINE         (0x40)
#define SSD1306_CMD_HW_CONFIG_SET_SEGMENT_REMAP              (0xA0)
#define SSD1306_CMD_HW_CONFIG_SET_MULTIPLEX_RATIO            (0xA8)
#define SSD1306_CMD_HW_CONFIG_SET_COM_SCAN_DIR_NORMAL        (0xC0)
#define SSD1306_CMD_HW_CONFIG_SET_COM_SCAN_DIR_REVERSE       (0xC8)
#define SSD1306_CMD_HW_CONFIG_SET_DISPLAY_OFFSET             (0xD3)
#define SSD1306_CMD_HW_CONFIG_SET_COM_PIN_CONFIG             (0xDA)

/* Timing & Driving Scheme Commands */
#define SSD1306_CMD_TIMING_SET_DISPLAY_CLK_DIVIDE_RATIO_AND_FREQ (0xD5)
#define SSD1306_CMD_TIMING_SET_PRECHARGE_PERIOD              (0xD9)
#define SSD1306_CMD_TIMING_SET_VCOM_DESELECT_LEVEL           (0xDB)
#define SSD1306_CMD_TIMING_SET_NOP                           (0xE3)

/* Charge Pump Commands */
#define SSD1306_CMD_CHARGE_PUMP_SET_CHARGE_PUMP             (0x8D)

/* Fundamental Commands Masks */
#define SSD1306_CMD_FUNDAMENTAL_SET_ENTIRE_DISPLAY_ON_MASK   (0x01)
#define SSD1306_CMD_FUNDAMENTAL_SET_DISPLAY_NORMAL_OR_INVERSE_MASK (0x01)
#define SSD1306_CMD_FUNDAMENTAL_SET_DISPLAY_ON_MASK             (0x01)

/* Scrolling Commands Masks */
#define SSD1306_CMD_SCROLLING_SET_SCROLL_PAGE_ADDR_MASK      (0x07)
#define SSD1306_CMD_SCROLLING_SET_SCROLL_OFFSET_MASK         (0x3F)
#define SSD1306_CMD_SCROLLING_SET_SCROLL_FIXED_AREA_MASK     (0x3F)
#define SSD1306_CMD_SCROLLING_SET_SCROLL_AREA_MASK           (0x7F)

/* Addressing Setting Commands Masks */
#define SSD1306_CMD_ADDRESSING_SET_LOW_COLUMN_ADDR_MASK      (0x0F)
#define SSD1306_CMD_ADDRESSING_SET_HIGH_COLUMN_ADDR_MASK     (0x1F)
#define SSD1306_CMD_ADDRESSING_SET_ADDR_MODE_MASK            (0x03)
#define SSD1306_CMD_ADDRESSING_SET_COLUMN_ADDR_MASK          (0x3F)
#define SSD1306_CMD_ADDRESSING_SET_PAGE_ADDR_MASK            (0x07)
#define SSD1306_CMD_ADDRESSING_SET_PAGE_START_ADDR_MASK      (0xB7)

/* Hardware Configuration Commands Masks */
#define SSD1306_CMD_HW_CONFIG_SET_DISPLAY_START_LINE_MASK    (0x3F)
#define SSD1306_CMD_HW_CONFIG_SET_SEGMENT_REMAP_MASK         (0x01)
#define SSD1306_CMD_HW_CONFIG_SET_MULTIPLEX_RATIO_MASK       (0x3F)
#define SSD1306_CMD_HW_CONFIG_SET_COM_SCAN_DIR_NORMAL_MASK   (0x01)
#define SSD1306_CMD_HW_CONFIG_SET_COM_SCAN_DIR_NORMAL_SHIFT  (0x03)
#define SSD1306_CMD_HW_CONFIG_SET_DISPLAY_OFFSET_MASK        (0x3F)
#define SSD1306_CMD_HW_CONFIG_SET_COM_PIN_CONFIG_MASK        (0x03)
#define SSD1306_CMD_HW_CONFIG_SET_COM_PIN_CONFIG_SHIFT       (0x04)

/* Charge Pump Commands Masks */
#define SSD1306_CMD_CHARGE_PUMP_SET_CHARGE_PUMP_MASK        (0x01)
#define SSD1306_CMD_CHARGE_PUMP_SET_CHARGE_PUMP_SHIFT       (0x02)

/* GDDRAM Page Address */
#define SSD1306_GDDRAM_ADDR_PAGE0                            (0x00)
#define SSD1306_GDDRAM_ADDR_PAGE1                            (0x01)
#define SSD1306_GDDRAM_ADDR_PAGE2                            (0x02)
#define SSD1306_GDDRAM_ADDR_PAGE3                            (0x03)
#define SSD1306_GDDRAM_ADDR_PAGE4                            (0x04)
#define SSD1306_GDDRAM_ADDR_PAGE5                            (0x05)
#define SSD1306_GDDRAM_ADDR_PAGE6                            (0x06)
#define SSD1306_GDDRAM_ADDR_PAGE7                            (0x07)

#define SSD1306_CMD_SUCCESS         (0)
#define SSD1306_CMD_I2C_WRITE_ERROR (-1)

static uint8_t ssd1306_data_buffer[256];

static int32_t ssd1306_i2c_write_cmd(ssd1306_reg_t* obj, const uint8_t* cmd, const uint8_t cmd_len) {
    return obj->i2c_write(obj->obj, cmd, cmd_len);
}

static int32_t ssd1306_i2c_read_data(ssd1306_reg_t* obj, uint8_t* data, uint8_t data_len) {
    return obj->i2c_read(obj->obj, data, data_len);
}

int32_t ssd1306_fundamental_set_contrast(ssd1306_reg_t* obj, const uint8_t contrast) {
    ssd1306_data_buffer[0] = SSD1306_CTRL_BYTE_CMD;
    ssd1306_data_buffer[1] = SSD1306_CMD_FUNDAMENTAL_SET_CONTRAST;
    ssd1306_data_buffer[2] = contrast;
    return ssd1306_i2c_write_cmd(obj, ssd1306_data_buffer, 3);
}

int32_t ssd1306_fundamental_set_column_address(ssd1306_reg_t* obj, const uint8_t column_address) {
    // Implementation needed (not present in current .c file)
    return SSD1306_CMD_SUCCESS;
}

int32_t ssd1306_fundamental_set_entire_display_on(ssd1306_reg_t* obj, const uint8_t ram_or_on) {
    ssd1306_data_buffer[0] = SSD1306_CTRL_BYTE_CMD;
    ssd1306_data_buffer[1] = SSD1306_CMD_FUNDAMENTAL_SET_ENTIRE_DISPLAY_ON | (SSD1306_CMD_FUNDAMENTAL_SET_ENTIRE_DISPLAY_ON_MASK & ram_or_on);
    return ssd1306_i2c_write_cmd(obj, ssd1306_data_buffer, 2);
}

int32_t ssd1306_fundamental_set_display_normal_or_inverse(ssd1306_reg_t* obj, const uint8_t normal_or_inverse) {
    ssd1306_data_buffer[0] = SSD1306_CTRL_BYTE_CMD;
    ssd1306_data_buffer[1] = SSD1306_CMD_FUNDAMENTAL_SET_DISPLAY_NORMAL_OR_INVERSE | (SSD1306_CMD_FUNDAMENTAL_SET_DISPLAY_NORMAL_OR_INVERSE_MASK & normal_or_inverse);
    return ssd1306_i2c_write_cmd(obj, ssd1306_data_buffer, 2);
}

int32_t ssd1306_fundamental_set_display_on(ssd1306_reg_t* obj, const uint8_t on) {
    ssd1306_data_buffer[0] = SSD1306_CTRL_BYTE_CMD;
    ssd1306_data_buffer[1] = SSD1306_CMD_FUNDAMENTAL_SET_DISPLAY_ON | (SSD1306_CMD_FUNDAMENTAL_SET_DISPLAY_ON_MASK & on);
    return ssd1306_i2c_write_cmd(obj, ssd1306_data_buffer, 2);
}

int32_t ssd1306_fundamental_set_display_off(ssd1306_reg_t* obj, const uint8_t on) {
    ssd1306_data_buffer[0] = SSD1306_CTRL_BYTE_CMD;
    ssd1306_data_buffer[1] = SSD1306_CMD_FUNDAMENTAL_SET_DISPLAY_ON | (SSD1306_CMD_FUNDAMENTAL_SET_DISPLAY_ON_MASK & 0x00);
    return ssd1306_i2c_write_cmd(obj, ssd1306_data_buffer, 2);
}

int32_t ssd1306_hw_config_set_display_start_line(ssd1306_reg_t* obj, const uint8_t start_line) {
    ssd1306_data_buffer[0] = SSD1306_CTRL_BYTE_CMD;
    ssd1306_data_buffer[1] = SSD1306_CMD_HW_CONFIG_SET_DISPLAY_START_LINE | (SSD1306_CMD_HW_CONFIG_SET_DISPLAY_START_LINE_MASK & start_line);
    return ssd1306_i2c_write_cmd(obj, ssd1306_data_buffer, 2);
}

int32_t ssd1306_hw_config_set_segment_remap(ssd1306_reg_t* obj, const uint8_t remap) {
    ssd1306_data_buffer[0] = SSD1306_CTRL_BYTE_CMD;
    ssd1306_data_buffer[1] = SSD1306_CMD_HW_CONFIG_SET_SEGMENT_REMAP | (SSD1306_CMD_HW_CONFIG_SET_SEGMENT_REMAP_MASK & remap);
    return ssd1306_i2c_write_cmd(obj, ssd1306_data_buffer, 2);
}

int32_t ssd1306_hw_config_set_multiplex_ratio(ssd1306_reg_t* obj, const uint8_t ratio) {
    ssd1306_data_buffer[0] = SSD1306_CTRL_BYTE_CMD;
    ssd1306_data_buffer[1] = SSD1306_CMD_HW_CONFIG_SET_MULTIPLEX_RATIO | (SSD1306_CMD_HW_CONFIG_SET_MULTIPLEX_RATIO_MASK & ratio);
    return ssd1306_i2c_write_cmd(obj, ssd1306_data_buffer, 2);
}

int32_t ssd1306_hw_config_set_com_output_scan_direction(ssd1306_reg_t* obj, const uint8_t scan_direction) {
    ssd1306_data_buffer[0] = SSD1306_CTRL_BYTE_CMD;
    ssd1306_data_buffer[1] = SSD1306_CMD_HW_CONFIG_SET_COM_SCAN_DIR_NORMAL | ((SSD1306_CMD_HW_CONFIG_SET_COM_SCAN_DIR_NORMAL_MASK & scan_direction) << SSD1306_CMD_HW_CONFIG_SET_COM_SCAN_DIR_NORMAL_SHIFT);
    return ssd1306_i2c_write_cmd(obj, ssd1306_data_buffer, 2);
}

int32_t ssd1306_hw_config_set_display_offset(ssd1306_reg_t* obj, const uint8_t offset) {
    ssd1306_data_buffer[0] = SSD1306_CTRL_BYTE_CMD;
    ssd1306_data_buffer[1] = SSD1306_CMD_HW_CONFIG_SET_DISPLAY_OFFSET;
    ssd1306_data_buffer[2] = SSD1306_CMD_HW_CONFIG_SET_DISPLAY_OFFSET_MASK & offset;
    return ssd1306_i2c_write_cmd(obj, ssd1306_data_buffer, 3);
}

int32_t ssd1306_hw_config_set_com_pin_config(ssd1306_reg_t* obj, const uint8_t pin_config) {
    ssd1306_data_buffer[0] = SSD1306_CTRL_BYTE_CMD;
    ssd1306_data_buffer[1] = SSD1306_CMD_HW_CONFIG_SET_COM_PIN_CONFIG;
    ssd1306_data_buffer[2] = 0x02 | ((SSD1306_CMD_HW_CONFIG_SET_COM_PIN_CONFIG_MASK & pin_config) << SSD1306_CMD_HW_CONFIG_SET_COM_PIN_CONFIG_SHIFT);
    return ssd1306_i2c_write_cmd(obj, ssd1306_data_buffer, 3);
}

int32_t ssd1306_timing_and_driving_scheme_set_display_clock_div(ssd1306_reg_t* obj, const uint8_t clock_div) {
    ssd1306_data_buffer[0] = SSD1306_CTRL_BYTE_CMD;
    ssd1306_data_buffer[1] = SSD1306_CMD_TIMING_SET_DISPLAY_CLK_DIVIDE_RATIO_AND_FREQ;
    ssd1306_data_buffer[2] = clock_div;
    return ssd1306_i2c_write_cmd(obj, ssd1306_data_buffer, 3);
}

int32_t ssd1306_charge_pump_set_charge_pump(ssd1306_reg_t* obj, const uint8_t enable) {
    ssd1306_data_buffer[0] = SSD1306_CTRL_BYTE_CMD;
    ssd1306_data_buffer[1] = SSD1306_CMD_CHARGE_PUMP_SET_CHARGE_PUMP;
    ssd1306_data_buffer[2] = 0x10 | ((SSD1306_CMD_CHARGE_PUMP_SET_CHARGE_PUMP_MASK & enable) << SSD1306_CMD_CHARGE_PUMP_SET_CHARGE_PUMP_SHIFT);
    return ssd1306_i2c_write_cmd(obj, ssd1306_data_buffer, 3);
}
