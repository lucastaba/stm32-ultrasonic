#ifndef SSD1306_REG_H
#define SSD1306_REG_H

#include <stdint.h>

#define SSD1306_CTRL_BYTE_CMD                                (0x00)
#define SSD1306_CTRL_BYTE_DATA                               (0x40)

/* Common Commands */
#define SSD1306_CMD_FUNDAMENTAL_SET_CONTRAST                 (0x81)
#define SSD1306_CMD_FUNDAMENTAL_SET_ENTIRE_DISPLAY_ON        (0xA4)
#define SSD1306_CMD_FUNDAMENTAL_SET_DISPLAY_NORMAL_OR_INVERSE (0xA6)
#define SSD1306_CMD_FUNDAMENTAL_SET_DISPLAY_ON               (0xAE)

/* Scrolling Commands */
#define SSD1306_CMD_SCROLLING_SET_SCROLL_H_SETUP             (0x26)
#define SSD1306_CMD_SCROLLING_SET_SCROLL_V_H_SETUP           (0x28)
#define SSD1306_CMD_SCROLLING_SET_SCROLL_STOP                (0x2E)
#define SSD1306_CMD_SCROLLING_SET_SCROLL_START               (0x2F)
#define SSD1306_CMD_SCROLLING_SET_SCROLL_VERTICAL_AREA       (0xA3)

/* Addressing Commands */
#define SSD1306_CMD_ADDRESSING_SET_LOWER_COLUMN_START_ADDR   (0X00)
#define SSD1306_CMD_ADDRESSING_SET_HIGHER_COLUMN_START_ADDR  (0X10)
#define SSD1306_CMD_ADDRESSING_SET_MEMORY_ADDR_MODE          (0x20)
#define SSD1306_CMD_ADDRESSING_SET_COLUMN_ADDR               (0x21)
#define SSD1306_CMD_ADDRESSING_SET_PAGE_ADDR                 (0x22)
#define SSD1306_CMD_ADDRESSING_SET_PAGE_START_ADDR           (0xB0)

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
#define SSD1306_CMD_CHARGE_PUMP_SET_CHARGE_PUMP              (0x8D)

/* Fundamental Commands Masks */
#define SSD1306_CMD_FUNDAMENTAL_SET_ENTIRE_DISPLAY_ON_MASK   (0x01)
#define SSD1306_CMD_FUNDAMENTAL_SET_DISPLAY_NORMAL_OR_INVERSE_MASK (0x01)
#define SSD1306_CMD_FUNDAMENTAL_SET_DISPLAY_ON_MASK          (0x01)

/* Scrolling Commands Masks */
#define SSD1306_CMD_SCROLLING_SET_SCROLL_H_SETUP_MASK        (0x01)
#define SSD1306_CMD_SCROLLING_SET_SCROLL_V_H_SETUP_MASK      (0x03)
#define SSD1306_CMD_SCROLLING_SET_SCROLL_PAGE_ADDR_MASK      (0x07)
#define SSD1306_CMD_SCROLLING_SET_SCROLL_INTERVAL_MASK       (0x07)
#define SSD1306_CMD_SCROLLING_SET_SCROLL_OFFSET_MASK         (0x3F)
#define SSD1306_CMD_SCROLLING_SET_SCROLL_FIXED_AREA_MASK     (0x3F)
#define SSD1306_CMD_SCROLLING_SET_SCROLL_AREA_MASK           (0x7F)

/* Addressing Setting Commands Masks */
#define SSD1306_CMD_ADDRESSING_SET_COLUMN_START_ADDR_MASK    (0x0F)
#define SSD1306_CMD_ADDRESSING_SET_MEMORY_ADDR_MODE_MASK     (0x03)
#define SSD1306_CMD_ADDRESSING_SET_COLUMN_ADDR_MASK          (0x7F)
#define SSD1306_CMD_ADDRESSING_SET_PAGE_ADDR_MASK            (0x07)
#define SSD1306_CMD_ADDRESSING_SET_PAGE_START_ADDR_MASK      (0x07)
#define SSD1306_CMD_ADDRESSING_LOWER_NIBBLE_MASK             (0x0F)
#define SSD1306_CMD_ADDRESSING_HIGHER_NIBBLE_MASK            (0xF0)

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
#define SSD1306_CMD_CHARGE_PUMP_SET_CHARGE_PUMP_MASK         (0x01)
#define SSD1306_CMD_CHARGE_PUMP_SET_CHARGE_PUMP_SHIFT        (0x02)

/* GDDRAM Page Address */
#define SSD1306_GDDRAM_ADDR_PAGE0                            (0x00)
#define SSD1306_GDDRAM_ADDR_PAGE1                            (0x01)
#define SSD1306_GDDRAM_ADDR_PAGE2                            (0x02)
#define SSD1306_GDDRAM_ADDR_PAGE3                            (0x03)
#define SSD1306_GDDRAM_ADDR_PAGE4                            (0x04)
#define SSD1306_GDDRAM_ADDR_PAGE5                            (0x05)
#define SSD1306_GDDRAM_ADDR_PAGE6                            (0x06)
#define SSD1306_GDDRAM_ADDR_PAGE7                            (0x07)

/* Dummy Byte */
#define SSD1306_CMD_DUMMY_BYTE_0                             (0x00)
#define SSD1306_CMD_DUMMY_BYTE_1                             (0xFF)

typedef int32_t (*SSD1306_Write_Func)(void* handle, const uint8_t reg, const uint8_t* data , const uint16_t len);
typedef int32_t (*SSD1306_Read_Func)(void* handle, const uint8_t reg, uint8_t* data, const uint16_t len);

typedef struct {
    void* handle;
    SSD1306_Write_Func WriteReg;
    SSD1306_Read_Func ReadReg;
} ssd1306_ctx_t;

/* Register Write/Read */
int32_t ssd1306_write_reg(ssd1306_ctx_t* ctx, const uint8_t reg, const uint8_t* data, const uint16_t len);
int32_t ssd1306_read_reg(ssd1306_ctx_t* ctx, const uint8_t reg, uint8_t* data, const uint16_t len);

/* Fundamental Commands */
int32_t ssd1306_fundamental_set_contrast(ssd1306_ctx_t* ctx, const uint8_t contrast);
int32_t ssd1306_fundamental_set_entire_display_on(ssd1306_ctx_t* ctx, const uint8_t ram_or_on);
int32_t ssd1306_fundamental_set_display_normal_or_inverse(ssd1306_ctx_t* ctx, const uint8_t normal_or_inverse);
int32_t ssd1306_fundamental_set_display_on(ssd1306_ctx_t* ctx, const uint8_t on);

/* Scrolling Commands */
int32_t ssd1306_scrolling_continuous_horizontal_scroll_setup(ssd1306_ctx_t* ctx, const uint8_t direction,const uint8_t start_page, const uint8_t end_page, const uint8_t interval);
int32_t ssd1306_scrolling_continuous_vertical_scroll_setup(ssd1306_ctx_t* ctx, const uint8_t direction,const uint8_t start_page, const uint8_t end_page, const uint8_t interval, const uint8_t offset);
int32_t ssd1306_scrolling_activate(ssd1306_ctx_t* ctx);
int32_t ssd1306_scrolling_deactivate(ssd1306_ctx_t* ctx);
int32_t ssd1306_scrolling_set_scroll_vertical_scroll_area(ssd1306_ctx_t* ctx, const uint8_t n_rows_top_fixed, const uint8_t n_rows);

/* Addressing Commands */
int32_t ssd1306_addressing_set_lower_column_start_address_for_page_addressing_mode(ssd1306_ctx_t* ctx, const uint8_t lower_nibble);
int32_t ssd1306_addressing_set_higher_column_start_address_for_page_addressing_mode(ssd1306_ctx_t* ctx, const uint8_t higher_nibble);
int32_t ssd1306_addressing_set_memory_addressing_mode(ssd1306_ctx_t* ctx, const uint8_t addressing_mode);
int32_t ssd1306_addressing_set_column_address(ssd1306_ctx_t* ctx, const uint8_t start_address, const uint8_t end_address);
int32_t ssd1306_addressing_set_page_address_for_horizotal_vertical_addressing_mode(ssd1306_ctx_t* ctx, const uint8_t start_page_address, const uint8_t end_page_address);
int32_t ssd1306_addressing_set_page_address_for_page_addressing_mode(ssd1306_ctx_t* ctx, const uint8_t start_page_address);

/* Hardware Configuration Commands */
int32_t ssd1306_hw_config_set_display_start_line(ssd1306_ctx_t* ctx, const uint8_t start_line);
int32_t ssd1306_hw_config_set_segment_remap(ssd1306_ctx_t* ctx, const uint8_t remap);
int32_t ssd1306_hw_config_set_multiplex_ratio(ssd1306_ctx_t* ctx, const uint8_t ratio);
int32_t ssd1306_hw_config_set_com_output_scan_direction(ssd1306_ctx_t* ctx, const uint8_t scan_direction);
int32_t ssd1306_hw_config_set_display_offset(ssd1306_ctx_t* ctx, const uint8_t offset);
int32_t ssd1306_hw_config_set_com_pin_config(ssd1306_ctx_t* ctx, const uint8_t pin_config);

/* Timing and Driving Scheme Commands */
int32_t ssd1306_timing_and_driving_scheme_set_display_clock_div_clock_freq(ssd1306_ctx_t* ctx, const uint8_t clock_div, const uint8_t clock_freq);
int32_t ssd1306_timing_and_driving_scheme_set_pre_charge_period(ssd1306_ctx_t* ctx, const uint8_t phase_1, const uint8_t phase_2);
int32_t ssd1306_timing_and_driving_scheme_set_vcomh_deselect_level(ssd1306_ctx_t* ctx, const uint8_t level);
int32_t ssd1306_timing_and_driving_scheme_nop(ssd1306_ctx_t* ctx);

/* Charge Pump Commands */
int32_t ssd1306_charge_pump_set_charge_pump(ssd1306_ctx_t* ctx, const uint8_t enable);

#endif /* SSD1306_REG_H */