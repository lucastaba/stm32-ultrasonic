#include <stdint.h>

#include "ssd1306_reg.h"

#define SSD1306_CMD_SUCCESS         (0)
#define SSD1306_CMD_I2C_WRITE_ERROR (-1)

int32_t ssd1306_write_reg(ssd1306_ctx_t* ctx, const uint8_t reg, const uint8_t* data, const uint16_t len) {
    return ctx->WriteReg(ctx->handle, reg, data, len);
}

int32_t ssd1306_read_reg(ssd1306_ctx_t* ctx, const uint8_t reg, uint8_t* data, const uint16_t len) {
    return ctx->ReadReg(ctx->handle, reg, data, len);
}

int32_t ssd1306_fundamental_set_contrast(ssd1306_ctx_t* ctx, const uint8_t contrast) {
    uint8_t data[] = {
        SSD1306_CMD_FUNDAMENTAL_SET_CONTRAST,
        contrast,
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_fundamental_set_entire_display_on(ssd1306_ctx_t* ctx, const uint8_t ram_or_on) {
    uint8_t data[] = {
        SSD1306_CMD_FUNDAMENTAL_SET_ENTIRE_DISPLAY_ON | (SSD1306_CMD_FUNDAMENTAL_SET_ENTIRE_DISPLAY_ON_MASK & ram_or_on),
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_fundamental_set_display_normal_or_inverse(ssd1306_ctx_t* ctx, const uint8_t normal_or_inverse) {
    uint8_t data[] = {
        SSD1306_CMD_FUNDAMENTAL_SET_DISPLAY_NORMAL_OR_INVERSE | (SSD1306_CMD_FUNDAMENTAL_SET_DISPLAY_NORMAL_OR_INVERSE_MASK & normal_or_inverse),
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_fundamental_set_display_on(ssd1306_ctx_t* ctx, const uint8_t on) {
    uint8_t data[] = {
        SSD1306_CMD_FUNDAMENTAL_SET_DISPLAY_ON | (SSD1306_CMD_FUNDAMENTAL_SET_DISPLAY_ON_MASK & on),
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_scrolling_continuous_horizontal_scroll_setup(ssd1306_ctx_t* ctx, const uint8_t direction,const uint8_t start_page, const uint8_t end_page, const uint8_t interval) {
    uint8_t data[] = {
        SSD1306_CMD_SCROLLING_SET_SCROLL_H_SETUP | (direction & SSD1306_CMD_SCROLLING_SET_SCROLL_H_SETUP_MASK),
        SSD1306_CMD_DUMMY_BYTE_0,
        (SSD1306_CMD_SCROLLING_SET_SCROLL_PAGE_ADDR_MASK & start_page),
        (SSD1306_CMD_SCROLLING_SET_SCROLL_INTERVAL_MASK & interval),
        (SSD1306_CMD_SCROLLING_SET_SCROLL_PAGE_ADDR_MASK & end_page),
        SSD1306_CMD_DUMMY_BYTE_0,
        SSD1306_CMD_DUMMY_BYTE_1,
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_scrolling_continuous_vertical_scroll_setup(ssd1306_ctx_t* ctx, const uint8_t direction,const uint8_t start_page, const uint8_t end_page, const uint8_t interval, const uint8_t offset) {
    uint8_t data[] = {
        SSD1306_CMD_SCROLLING_SET_SCROLL_V_H_SETUP | (SSD1306_CMD_SCROLLING_SET_SCROLL_V_H_SETUP_MASK & direction),
        SSD1306_CMD_DUMMY_BYTE_0,
        (SSD1306_CMD_SCROLLING_SET_SCROLL_PAGE_ADDR_MASK & start_page),
        (SSD1306_CMD_SCROLLING_SET_SCROLL_INTERVAL_MASK & interval),
        (SSD1306_CMD_SCROLLING_SET_SCROLL_PAGE_ADDR_MASK & end_page),
        (SSD1306_CMD_SCROLLING_SET_SCROLL_OFFSET_MASK & offset),
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_scrolling_activate(ssd1306_ctx_t* ctx) {
    uint8_t data[] = {
        SSD1306_CMD_SCROLLING_SET_SCROLL_START,
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_scrolling_deactivate(ssd1306_ctx_t* ctx) {
    uint8_t data[] = {
        SSD1306_CMD_SCROLLING_SET_SCROLL_STOP,
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_scrolling_set_scroll_vertical_scroll_area(ssd1306_ctx_t* ctx, const uint8_t n_rows_top_fixed, const uint8_t n_rows) {
    uint8_t data[] = {
        SSD1306_CMD_SCROLLING_SET_SCROLL_VERTICAL_AREA,
        (SSD1306_CMD_SCROLLING_SET_SCROLL_FIXED_AREA_MASK & n_rows_top_fixed),
        (SSD1306_CMD_SCROLLING_SET_SCROLL_AREA_MASK & n_rows),
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_addressing_set_lower_column_start_address_for_page_addressing_mode(ssd1306_ctx_t* ctx, const uint8_t lower_nibble) {
    uint8_t data[] = {
        SSD1306_CMD_ADDRESSING_SET_LOWER_COLUMN_START_ADDR | (SSD1306_CMD_ADDRESSING_SET_COLUMN_START_ADDR_MASK & lower_nibble),
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_addressing_set_higher_column_start_address_for_page_addressing_mode(ssd1306_ctx_t* ctx, const uint8_t higher_nibble) {
    uint8_t data[] = {
        SSD1306_CMD_ADDRESSING_SET_HIGHER_COLUMN_START_ADDR | (SSD1306_CMD_ADDRESSING_SET_COLUMN_START_ADDR_MASK & higher_nibble),
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_addressing_set_memory_addressing_mode(ssd1306_ctx_t* ctx, const uint8_t addressing_mode) {
    uint8_t data[] = {
        SSD1306_CMD_ADDRESSING_SET_MEMORY_ADDR_MODE,
        (SSD1306_CMD_ADDRESSING_SET_MEMORY_ADDR_MODE_MASK & addressing_mode),
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_addressing_set_column_address(ssd1306_ctx_t* ctx, const uint8_t start_address, const uint8_t end_address) {
    uint8_t data[] = {
        SSD1306_CMD_ADDRESSING_SET_COLUMN_ADDR,
        (SSD1306_CMD_ADDRESSING_SET_COLUMN_ADDR_MASK & start_address),
        (SSD1306_CMD_ADDRESSING_SET_COLUMN_ADDR_MASK & end_address),
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_addressing_set_page_address_for_horizotal_vertical_addressing_mode(ssd1306_ctx_t* ctx, const uint8_t start_page_address, const uint8_t end_page_address) {
    uint8_t data[] = {
        SSD1306_CMD_ADDRESSING_SET_PAGE_ADDR,
        (SSD1306_CMD_ADDRESSING_SET_PAGE_ADDR_MASK & start_page_address),
        (SSD1306_CMD_ADDRESSING_SET_PAGE_ADDR_MASK & end_page_address),
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_addressing_set_page_address_for_page_addressing_mode(ssd1306_ctx_t* ctx, const uint8_t start_address) {
    uint8_t data[] = {
        SSD1306_CMD_ADDRESSING_SET_PAGE_START_ADDR | (SSD1306_CMD_ADDRESSING_SET_PAGE_START_ADDR_MASK & start_address),
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_hw_config_set_display_start_line(ssd1306_ctx_t* ctx, const uint8_t start_line) {
    uint8_t data[] = {
        SSD1306_CMD_HW_CONFIG_SET_DISPLAY_START_LINE | (SSD1306_CMD_HW_CONFIG_SET_DISPLAY_START_LINE_MASK & start_line),
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_hw_config_set_segment_remap(ssd1306_ctx_t* ctx, const uint8_t remap) {
    uint8_t data[] = {
        SSD1306_CMD_HW_CONFIG_SET_SEGMENT_REMAP | (SSD1306_CMD_HW_CONFIG_SET_SEGMENT_REMAP_MASK & remap),
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_hw_config_set_multiplex_ratio(ssd1306_ctx_t* ctx, const uint8_t ratio) {
    uint8_t data[] = {
        SSD1306_CMD_HW_CONFIG_SET_MULTIPLEX_RATIO | (SSD1306_CMD_HW_CONFIG_SET_MULTIPLEX_RATIO_MASK & ratio),
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_hw_config_set_com_output_scan_direction(ssd1306_ctx_t* ctx, const uint8_t scan_direction) {
    uint8_t data[] = {
        SSD1306_CMD_HW_CONFIG_SET_COM_SCAN_DIR_NORMAL | ((SSD1306_CMD_HW_CONFIG_SET_COM_SCAN_DIR_NORMAL_MASK & scan_direction) << SSD1306_CMD_HW_CONFIG_SET_COM_SCAN_DIR_NORMAL_SHIFT),
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_hw_config_set_display_offset(ssd1306_ctx_t* ctx, const uint8_t offset) {
    uint8_t data[] = {
        SSD1306_CMD_HW_CONFIG_SET_DISPLAY_OFFSET,
        SSD1306_CMD_HW_CONFIG_SET_DISPLAY_OFFSET_MASK & offset,
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_hw_config_set_com_pin_config(ssd1306_ctx_t* ctx, const uint8_t pin_config) {
    uint8_t data[] = {
        SSD1306_CMD_HW_CONFIG_SET_COM_PIN_CONFIG,
        0x02 | ((SSD1306_CMD_HW_CONFIG_SET_COM_PIN_CONFIG_MASK & pin_config) << SSD1306_CMD_HW_CONFIG_SET_COM_PIN_CONFIG_SHIFT),
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_timing_and_driving_scheme_set_display_clock_div_clock_freq(ssd1306_ctx_t* ctx, const uint8_t clock_div, const uint8_t clock_freq) {
    uint8_t data[] = {
        SSD1306_CMD_TIMING_SET_DISPLAY_CLK_DIVIDE_RATIO_AND_FREQ,
        ((uint8_t)(clock_freq << 4) & 0xF0) | (uint8_t)(clock_div & 0x0F),
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_timing_and_driving_scheme_set_pre_charge_period(ssd1306_ctx_t* ctx, const uint8_t phase_1, const uint8_t phase_2) {
    uint8_t data[] = {
        SSD1306_CMD_TIMING_SET_PRECHARGE_PERIOD,
        (phase_2 << 4) | phase_1,
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_timing_and_driving_scheme_set_vcomh_deselect_level(ssd1306_ctx_t* ctx, const uint8_t level) {
    uint8_t data[] = {
        SSD1306_CMD_TIMING_SET_VCOM_DESELECT_LEVEL,
        (level & 0x07) << 4,
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_timing_and_driving_scheme_nop(ssd1306_ctx_t* ctx) {
    uint8_t data[] = {
        SSD1306_CMD_TIMING_SET_NOP,
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_charge_pump_set_charge_pump(ssd1306_ctx_t* ctx, const uint8_t enable) {
    uint8_t data[] = {
        SSD1306_CMD_CHARGE_PUMP_SET_CHARGE_PUMP,
        0x10 | ((SSD1306_CMD_CHARGE_PUMP_SET_CHARGE_PUMP_MASK & enable) << SSD1306_CMD_CHARGE_PUMP_SET_CHARGE_PUMP_SHIFT),
    };
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_CMD, data, sizeof(data));
}

int32_t ssd1306_gddram_write_data(ssd1306_ctx_t* ctx, const uint8_t* data, const uint16_t data_size) {
    return ssd1306_write_reg(ctx, SSD1306_CTRL_BYTE_DATA, data, data_size);
}