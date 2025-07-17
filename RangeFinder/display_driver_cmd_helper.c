#include <stdint.h>
#include <string.h>

#define DISPLAY_DRIVER_MAX_CMD_SIZE (4U)

#define DISPLAY_DRIVER_SUCCESS (0)
#define DISPLAY_DRIVER_INVALID_CMD_TYPE (-1)

typedef enum {
    /* Fundamental Commands */
    SET_CONTRAST = 0U,
    SET_COMLUMN_ADDRESS,
    SET_ENTINE_DISPLAY_ON,
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
    {SET_CONTRAST, 0, 0},
    {SET_COMLUMN_ADDRESS, 0, 0},
    {SET_ENTINE_DISPLAY_ON, 0, 0},
    {SET_DISPLAY_NORMAL_OR_INVERSE, 0, 0},
    {SET_DISPLAY_ON, 0, 0},

    /* Scrolling Commands */
    {SET_CONTINUOUS_HORIZONTAL_SCROLL_SETUP, 0, 0},
    {SET_CONTINUOUS_VERITICAL_SCROLL_SETUP, 0, 0},
    {SET_SCROLLING_ACTIVVATE, 0, 0},
    {SET_SCROLLING_DEACTIVATE, 0, 0},
    {SET_SCROLLING_VERTICAL_SCROLL_AREA, 0, 0},

    /* Addressing Commands */
    {SET_PAGE_ADDRESS_FOR_HORIZONTAL_VERTICAL_ADDRESSING_MODE, 0, 0},
    {SET_PAGE_ADDRESS_FOR_PAGE_ADDRESSING_MODE, 0, 0},

    /* Hardware Configuration Commands */
    {SET_HW_CONFIG_SET_DISPLAY_START_LINE, 0, 0},
    {SET_HW_CONFIG_SET_SEGMENT_REMAP, 0, 0},
    {SET_HW_CONFIG_SET_MULTIPLEX_RATIO, 0, 0},
    {SET_HW_CONFIG_SET_COM_OUTPUT_SCAN_DIRECTION, 0, 0},
    {SET_HW_CONFIG_SET_DISPLAY_OFFSET, 0, 0},
    {SET_HW_CONFIG_SET_COM_PIN_CONFIG, 0, 0},

    /* Timing and Driving Scheme Commands */
    {SET_TIMING_AND_DRIVING_SCHEME_SET_DISPLAY_CLOCK_DIV, 0, 0},
    {SET_TIMING_AND_DRIVING_SCHEME_SET_PRE_CHARGE_PERIOD, 0, 0},
    {SET_TIMING_AND_DRIVING_SCHEME_SET_VCOMH_DESELECT_LEVEL, 0, 0},
    {SET_TIMING_AND_DRIVING_SCHEME_SET_NOP, 0, 0},

    /* Charge Pump Commands */
    {SET_CHARGE_PUMP_SET_CHARGE_PUMP, 0, 0},
};

/* API prototypes to handle commands */
int32_t display_driver_get_cmd_len(uint8_t cmd_type);
int32_t display_driver_populate_cmd_struct(uint8_t cmd_type, uint8_t* data, DISPLAY_DRIVER_CMD_t* cmd);
int32_t display_driver_exec_cmd(DISPLAY_DRIVER_CMD_t* cmd);

/* Private prototypes */

/* API Implementation */
int32_t display_driver_get_cmd_len(uint8_t cmd_type) {
    if (cmd_type >= MAX_CMD_LEN) {
        return DISPLAY_DRIVER_INVALID_CMD_TYPE;
    }
    return display_driver_cmd_table[(DISPLAY_DRIVER_CMD_TYPE_t)cmd_type].cmd_size;
}

int32_t display_driver_populate_cmd_struct(uint8_t cmd_type, uint8_t* data, DISPLAY_DRIVER_CMD_t* cmd) {
    if (cmd_type >= MAX_CMD_LEN) {
        return DISPLAY_DRIVER_INVALID_CMD_TYPE;
    }

    int i = 0;
    cmd->type = display_driver_cmd_table[(DISPLAY_DRIVER_CMD_TYPE_t)cmd_type].type;
    cmd->cmd_size = display_driver_cmd_table[(DISPLAY_DRIVER_CMD_TYPE_t)cmd_type].cmd_size;
    memcpy(cmd->data, data, cmd->cmd_size);

    return DISPLAY_DRIVER_SUCCESS;
}

int32_t display_driver_exec_cmd(DISPLAY_DRIVER_CMD_t* cmd) {
    return DISPLAY_DRIVER_SUCCESS;
}

/* Private Implementation */