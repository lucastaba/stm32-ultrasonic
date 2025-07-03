#include <stdint.h>

typedef int32_t (*i2c_write_fptr)(void* obj, const uint8_t* cmd ,const uint8_t cmd_len);
typedef int32_t (*i2c_read_fptr)(void* obj, uint8_t* data, const uint8_t data_len);
typedef struct {
    void* obj;
    i2c_write_fptr i2c_write;
    i2c_read_fptr i2c_read;
} ssd1306_reg_t;

/* Fundamental Commands */
int32_t ssd1306_fundamental_set_contrast(ssd1306_reg_t* obj, uint8_t contrast_value);
int32_t ssd1306_fundamental_set_column_address(ssd1306_reg_t* obj, uint8_t contrast_value);
int32_t ssd1306_fundamental_set_entire_display_on(ssd1306_reg_t* obj, const uint8_t on);
int32_t ssd1306_fundamental_set_display_normal_or_inverse(ssd1306_reg_t* obj, const uint8_t normal);
int32_t ssd1306_fundamental_set_display_on(ssd1306_reg_t* obj, const uint8_t on);

/* Hardware Configuration Commands */
int32_t ssd1306_hw_config_set_display_start_line(ssd1306_reg_t* obj, const uint8_t display_start_line);
int32_t ssd1306_hw_config_set_segment_remap(ssd1306_reg_t* obj, const uint8_t segment_remap);
int32_t ssd1306_hw_config_set_multiplex_ratio(ssd1306_reg_t* obj, const uint8_t multiplex_ratio);
int32_t ssd1306_hw_config_set_com_output_scan_direction(ssd1306_reg_t* obj, const uint8_t com_output_scan_direction);
int32_t ssd1306_hw_config_set_display_offset(ssd1306_reg_t* obj, const uint8_t display_offset);
int32_t ssd1306_hw_config_set_com_pin_config(ssd1306_reg_t* obj, const uint8_t com_pin_config);