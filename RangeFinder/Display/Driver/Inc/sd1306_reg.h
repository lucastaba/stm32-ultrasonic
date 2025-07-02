#include <stdint.h>

typedef int32_t (*i2c_write_fptr)(const uint8_t* cmd ,const uint8_t cmd_len);
typedef int32_t (*i2c_read_fptr)(uint8_t* data, const uint8_t data_len);

typedef struct {
    i2c_write_fptr i2c_write;
    i2c_read_fptr i2c_read;
} sd1306_reg_t;

int32_t sd1306_fundamental_set_column_address(sd1306_reg_t* obj, uint8_t contrast_value);
int32_t sd1306_fundamental_set_display_resume_to_ram(sd1306_reg_t* obj);
int32_t sd1306_fundamental_set_display_all_on(sd1306_reg_t* obj);
int32_t sd1306_fundamental_set_display_normal(sd1306_reg_t* obj);
int32_t sd1306_fundamental_set_display_inverse(sd1306_reg_t* obj);
int32_t sd1306_fundamental_set_display_off(sd1306_reg_t* obj);
int32_t sd1306_fundamental_set_display_on(sd1306_reg_t* obj);