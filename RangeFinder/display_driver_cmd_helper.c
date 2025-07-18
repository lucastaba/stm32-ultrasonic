#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>

#include "Driver_I2C.h"

#include "ssd1306_reg.h"

#define DISPLAY_DRIVER_MAX_CMD_SIZE (4U)
#define DISPLAY_DRIVER_CMD_BUF_SIZE (256U)
#define DISPLAY_DRIVER_MAX_CMD_LEN (DISPLAY_DRIVER_CMD_BUF_SIZE - 1U)

#define DISPLAY_DRIVER_SUCCESS (0)
#define DISPLAY_DRIVER_INVALID_CMD_TYPE (-1)
#define DISPLAY_DRIVER_CMD_TOO_BIG (-2)
#define DISPLAY_DRIVER_PARSE_FAILED (-3)
#define DISPLAY_DRIVER_CMD_NOT_FOUND (-4)
#define DISPLAY_DRIVER_NOT_ENOUGH_ARGS (-5)
#define DISPLAY_DRIVER_INVALID_DATA (-6)
#define DISPLAY_DRIVER_EXEC_FAILED (-7)

typedef struct {
    ARM_DRIVER_I2C* pDrv;
    ssd1306_reg_t reg;
} ssd1306_obj_t;

typedef enum {
    /* Fundamental Commands */
    SET_CONTRAST = 0U,
    SET_ENTIRE_DISPLAY_ON,
    SET_DISPLAY_NORMAL_OR_INVERSE,
    SET_DISPLAY_ON,

    /* Scrolling Commands */
    SET_CONTINUOUS_HORIZONTAL_SCROLL_SETUP,
    SET_CONTINUOUS_VERITICAL_SCROLL_SETUP,
    SET_SCROLLING_ACTIVVATE,
    SET_SCROLLING_DEACTIVATE,
    SET_SCROLLING_VERTICAL_SCROLL_AREA,

    /* Addressing Commands */
    SET_PAGE_ADDRESS_FOR_HORIZONTAL_VERTICAL_ADDRESSING_MODE,
    SET_PAGE_ADDRESS_FOR_PAGE_ADDRESSING_MODE,

    /* Hardware Configuration Commands */
    SET_HW_CONFIG_SET_DISPLAY_START_LINE,
    SET_HW_CONFIG_SET_SEGMENT_REMAP,
    SET_HW_CONFIG_SET_MULTIPLEX_RATIO,
    SET_HW_CONFIG_SET_COM_OUTPUT_SCAN_DIRECTION,
    SET_HW_CONFIG_SET_DISPLAY_OFFSET,
    SET_HW_CONFIG_SET_COM_PIN_CONFIG,

    /* Timing and Driving Scheme Commands */
    SET_TIMING_AND_DRIVING_SCHEME_SET_DISPLAY_CLOCK_DIV,
    SET_TIMING_AND_DRIVING_SCHEME_SET_PRE_CHARGE_PERIOD,
    SET_TIMING_AND_DRIVING_SCHEME_SET_VCOMH_DESELECT_LEVEL,
    SET_TIMING_AND_DRIVING_SCHEME_SET_NOP,

    /* Charge Pump Commands */
    SET_CHARGE_PUMP_SET_CHARGE_PUMP,

    MAX_CMD_LEN,
    LAST = 0xFF,
} DISPLAY_DRIVER_CMD_TYPE_t;

typedef uint8_t CMD_SIZE_t;
typedef uint8_t CMD_DATA_t;

typedef struct {
    DISPLAY_DRIVER_CMD_TYPE_t type;
    CMD_SIZE_t cmd_size;
    CMD_DATA_t data[DISPLAY_DRIVER_MAX_CMD_SIZE];
} DISPLAY_DRIVER_CMD_t;

static const DISPLAY_DRIVER_CMD_t display_driver_cmd_table[MAX_CMD_LEN] = {
    /* Fundamental Commands */
    {SET_CONTRAST, 1, {0}},
    {SET_ENTIRE_DISPLAY_ON, 1, {0}},
    {SET_DISPLAY_NORMAL_OR_INVERSE, 1, {0}},
    {SET_DISPLAY_ON, 1, {0}},

    /* Scrolling Commands */
    {SET_CONTINUOUS_HORIZONTAL_SCROLL_SETUP, 0, {0}},
    {SET_CONTINUOUS_VERITICAL_SCROLL_SETUP, 0, {0}},
    {SET_SCROLLING_ACTIVVATE, 0, {0}},
    {SET_SCROLLING_DEACTIVATE, 0, {0}},
    {SET_SCROLLING_VERTICAL_SCROLL_AREA, 0, {0}},

    /* Addressing Commands */
    {SET_PAGE_ADDRESS_FOR_HORIZONTAL_VERTICAL_ADDRESSING_MODE, 0, {0}},
    {SET_PAGE_ADDRESS_FOR_PAGE_ADDRESSING_MODE, 0, {0}},

    /* Hardware Configuration Commands */
    {SET_HW_CONFIG_SET_DISPLAY_START_LINE, 1, {0}},
    {SET_HW_CONFIG_SET_SEGMENT_REMAP, 1, {0}},
    {SET_HW_CONFIG_SET_MULTIPLEX_RATIO, 1, {0}},
    {SET_HW_CONFIG_SET_COM_OUTPUT_SCAN_DIRECTION, 1, {0}},
    {SET_HW_CONFIG_SET_DISPLAY_OFFSET, 1, {0}},
    {SET_HW_CONFIG_SET_COM_PIN_CONFIG, 2, {0}},

    /* Timing and Driving Scheme Commands */
    {SET_TIMING_AND_DRIVING_SCHEME_SET_DISPLAY_CLOCK_DIV, 2, {0}},
    {SET_TIMING_AND_DRIVING_SCHEME_SET_PRE_CHARGE_PERIOD, 2, {0}},
    {SET_TIMING_AND_DRIVING_SCHEME_SET_VCOMH_DESELECT_LEVEL, 1, {0}},
    {SET_TIMING_AND_DRIVING_SCHEME_SET_NOP, 0, {0}},

    /* Charge Pump Commands */
    {SET_CHARGE_PUMP_SET_CHARGE_PUMP, 1, {0}},
};

static const char* display_driver_cmd_str_table[MAX_CMD_LEN] = {
    /* Fundamental Commands */
    "SET_CONTRAST",
    "SET_ENTIRE_DISPLAY_ON",
    "SET_DISPLAY_NORMAL_OR_INVERSE",
    "SET_DISPLAY_ON",

    /* Scrolling Commands */
    "SET_CONTINUOUS_HORIZONTAL_SCROLL_SETUP",
    "SET_CONTINUOUS_VERITICAL_SCROLL_SETUP",
    "SET_SCROLLING_ACTIVVATE",
    "SET_SCROLLING_DEACTIVATE",
    "SET_SCROLLING_VERTICAL_SCROLL_AREA",

    /* Addressing Commands */
    "SET_PAGE_ADDRESS_FOR_HORIZONTAL_VERTICAL_ADDRESSING_MODE",
    "SET_PAGE_ADDRESS_FOR_PAGE_ADDRESSING_MODE",

    /* Hardware Configuration Commands */
    "SET_HW_CONFIG_SET_DISPLAY_START_LINE",
    "SET_HW_CONFIG_SET_SEGMENT_REMAP",
    "SET_HW_CONFIG_SET_MULTIPLEX_RATIO",
    "SET_HW_CONFIG_SET_COM_OUTPUT_SCAN_DIRECTION",
    "SET_HW_CONFIG_SET_DISPLAY_OFFSET",
    "SET_HW_CONFIG_SET_COM_PIN_CONFIG",

    /* Timing and Driving Scheme Commands */
    "SET_TIMING_AND_DRIVING_SCHEME_SET_DISPLAY_CLOCK_DIV",
    "SET_TIMING_AND_DRIVING_SCHEME_SET_PRE_CHARGE_PERIOD",
    "SET_TIMING_AND_DRIVING_SCHEME_SET_VCOMH_DESELECT_LEVEL",
    "SET_TIMING_AND_DRIVING_SCHEME_SET_NOP",

    /* Charge Pump Commands */
    "SET_CHARGE_PUMP_SET_CHARGE_PUMP",
};

/* API prototypes to handle commands */
int32_t display_driver_parse_and_exec_cmd(ssd1306_obj_t* obj, uint8_t* data, uint16_t len);

/* Private prototypes */
static uint8_t cmd_buf[DISPLAY_DRIVER_CMD_BUF_SIZE];
static uint16_t cmd_len;

static void display_driver_copy_data_to_internal_buf(uint8_t* data, uint16_t len);
static void display_driver_clean_internal_buf();
static int32_t display_driver_parse_cmd(DISPLAY_DRIVER_CMD_t* cmd);
static int32_t display_driver_exec_cmd(ssd1306_obj_t* obj, DISPLAY_DRIVER_CMD_t* cmd);
static int32_t display_driver_search_cmd(const char* cmd_str);
static void display_driver_internal_buf_str_to_upper();

/* API Implementation */
int32_t display_driver_parse_and_exec_cmd(ssd1306_obj_t* obj, uint8_t* data, uint16_t len) {
    DISPLAY_DRIVER_CMD_t cmd;

    if (len > DISPLAY_DRIVER_MAX_CMD_LEN) {
        return DISPLAY_DRIVER_CMD_TOO_BIG;
    }

    display_driver_clean_internal_buf();
    display_driver_copy_data_to_internal_buf(data, len);
    display_driver_internal_buf_str_to_upper();

    if (display_driver_parse_cmd(&cmd) != DISPLAY_DRIVER_SUCCESS) {
        return DISPLAY_DRIVER_PARSE_FAILED;
    }

    if (display_driver_exec_cmd(obj, &cmd) != DISPLAY_DRIVER_SUCCESS) {
        return DISPLAY_DRIVER_EXEC_FAILED;
    }

    return DISPLAY_DRIVER_SUCCESS;
}

static void display_driver_copy_data_to_internal_buf(uint8_t* data, uint16_t len) {
    cmd_len = len;
    memcpy(cmd_buf, data, cmd_len);
}
static void display_driver_clean_internal_buf() {
    memset(cmd_buf, 0, DISPLAY_DRIVER_CMD_BUF_SIZE);
}

static int32_t display_driver_parse_cmd(DISPLAY_DRIVER_CMD_t* cmd) {
    const char* token_delimiter = " ";
    char* cmd_token;
    char* data_token;
    char* data_tokens[DISPLAY_DRIVER_MAX_CMD_SIZE];
    int32_t n_data_tokens = 0;
    int32_t ret;
    DISPLAY_DRIVER_CMD_TYPE_t cmd_type;
    uint8_t data[DISPLAY_DRIVER_MAX_CMD_SIZE];
    uint8_t cmd_size;
    long int conv_data;
    int i;

    cmd_token = strtok((char*)cmd_buf, token_delimiter);
    while ((data_token = strtok(NULL, token_delimiter)) != NULL && n_data_tokens < DISPLAY_DRIVER_MAX_CMD_SIZE) {
        data_tokens[n_data_tokens++] = data_token;
    }

    ret = display_driver_search_cmd(cmd_token);
    if (ret < 0) {
        return DISPLAY_DRIVER_CMD_NOT_FOUND;
    }

    cmd_type = (DISPLAY_DRIVER_CMD_TYPE_t)ret;
    if (n_data_tokens < display_driver_cmd_table[cmd_type].cmd_size) {
        return DISPLAY_DRIVER_NOT_ENOUGH_ARGS;
    }

    cmd_size = display_driver_cmd_table[cmd_type].cmd_size;
    for (i = 0; i < cmd_size; i++) {
        conv_data = strtol(data_tokens[i], NULL, 0);
        if (conv_data > UINT8_MAX) {
            return DISPLAY_DRIVER_INVALID_DATA;
        }
        data[i] = (uint8_t)conv_data;
    }

    cmd->type = cmd_type;
    cmd->cmd_size = cmd_size;
    for (i = 0; i < cmd_size; i++) {
        cmd->data[i] = data[i];
    }

    return DISPLAY_DRIVER_SUCCESS;
}

static int32_t display_driver_exec_cmd(ssd1306_obj_t* obj, DISPLAY_DRIVER_CMD_t* cmd) {
    int32_t ret;
    switch (cmd->type) {
        case SET_CONTRAST:
            ret = ssd1306_fundamental_set_contrast(&(obj->reg), cmd->data[0]);
            break;
        case SET_ENTIRE_DISPLAY_ON:
            ret = ssd1306_fundamental_set_entire_display_on(&(obj->reg), cmd->data[0]);
            break;
        case SET_DISPLAY_NORMAL_OR_INVERSE:
            ret = ssd1306_fundamental_set_display_normal_or_inverse(&(obj->reg), cmd->data[0]);
            break;
        case SET_DISPLAY_ON:
            ret = ssd1306_fundamental_set_display_on(&(obj->reg), cmd->data[0]);
            break;
        case SET_HW_CONFIG_SET_DISPLAY_START_LINE:
            ret = ssd1306_hw_config_set_display_start_line(&(obj->reg), cmd->data[0]);
            break;
        case SET_HW_CONFIG_SET_SEGMENT_REMAP:
            ret = ssd1306_hw_config_set_segment_remap(&(obj->reg), cmd->data[0]);
            break;
        case SET_HW_CONFIG_SET_MULTIPLEX_RATIO:
            ret = ssd1306_hw_config_set_multiplex_ratio(&(obj->reg), cmd->data[0]);
            break;
        case SET_HW_CONFIG_SET_COM_OUTPUT_SCAN_DIRECTION:
            ret = ssd1306_hw_config_set_com_output_scan_direction(&(obj->reg), cmd->data[0]);
            break;
        case SET_HW_CONFIG_SET_DISPLAY_OFFSET:
            ret = ssd1306_hw_config_set_display_offset(&(obj->reg), cmd->data[0]);
            break;
        case SET_HW_CONFIG_SET_COM_PIN_CONFIG:
            ret = ssd1306_hw_config_set_com_pin_config(&(obj->reg), cmd->data[0]);
            break;
        case SET_TIMING_AND_DRIVING_SCHEME_SET_DISPLAY_CLOCK_DIV:
            ret = ssd1306_timing_and_driving_scheme_set_display_clock_div_clock_freq(&(obj->reg), cmd->data[0], cmd->data[1]);
            break;
        case SET_TIMING_AND_DRIVING_SCHEME_SET_PRE_CHARGE_PERIOD:
            ret = ssd1306_timing_and_driving_scheme_set_pre_charge_period(&(obj->reg), cmd->data[0], cmd->data[1]);
            break;
        case SET_TIMING_AND_DRIVING_SCHEME_SET_VCOMH_DESELECT_LEVEL:
            ret = ssd1306_timing_and_driving_scheme_set_vcomh_deselect_level(&(obj->reg), cmd->data[0]);
            break;
        case SET_TIMING_AND_DRIVING_SCHEME_SET_NOP:
            ret = ssd1306_timing_and_driving_scheme_nop(&(obj->reg));
            break;
        default:
            break;
    }
    return ret;
}

static int32_t display_driver_search_cmd(const char* cmd_str) {
    int i = 0;
    for (; i < MAX_CMD_LEN; i++) {
        if (strcmp(cmd_str, display_driver_cmd_str_table[i]) == 0) {
            return i;
        }
    }
    return DISPLAY_DRIVER_INVALID_CMD_TYPE;
}

static void display_driver_internal_buf_str_to_upper() {
    char* tmp = (char*)cmd_buf;
    for (; *tmp; tmp++) {
        *tmp = toupper(*tmp);
    }
}

/* Private Implementation */