#include <string.h>

#include "ssd1306.h"
#include "ssd1306_reg.h"

#define SSD1306_DISPLAY_START_LINE_MASK (0x3FU)
#define SSD1306_DISPLAY_MULTIPLEX_RATIO_MASK (0x3FU)
#define SSD1306_DISPLAY_OFFSET_MASK (0x3FU)
#define SSD1306_DISPLAY_CLOCK_MASK (0x0FU)
#define SSD1306_DISPLAY_CLOCK_DIV_MASK (0x0FU)
#define SSD1306_DISPLAY_PHASE_PERIOD_MASK (0x0FU)

#define SSD1306_DISPLAY_PIN_DIRECTION_BIT_SHIFT (0x01U)

#define SSD1306_DISPLAY_MULTIPLEX_RATIO_MIN (0x0FU)

#define SSD1306_SUCCESS (0)
#define SSD1306_CMD_FAILED (-1)

static int32_t SSD1306_WriteRegWrap(void* handle, const uint8_t reg, const uint8_t* data , const uint16_t len) {
    SSD1306_Object_t* obj = (SSD1306_Object_t*)(handle);
    return obj->io.WriteReg(obj->io.i2cAddress, reg, data, len);
}
static int32_t SSD1306_ReadReagWrap(void* handle, const uint8_t reg, uint8_t* data, const uint16_t len) {
    SSD1306_Object_t* obj = (SSD1306_Object_t*)(handle);
    return obj->io.ReadReg(obj->io.i2cAddress, reg, data, len);
}

int32_t SSD1306_Init(SSD1306_Object_t* obj) {
    SSD1306_DISPLAY_STATE_t ds = obj->displayState;

    if (obj->isInitialized) {
        return SSD1306_SUCCESS;
    }

    if (SSD1306_SetDisplayState(obj, SLEEP) < 0) {
        return SSD1306_CMD_FAILED;
    }

    if (SSD1306_SetContrast(obj, obj->contrast) < 0) {
        return SSD1306_CMD_FAILED;
    }

    if (SSD1306_SetDisplayContent(obj, obj->content) < 0) {
        return SSD1306_CMD_FAILED;
    }

    if (SSD1306_SetDisplayColorInverse(obj, obj->color) < 0) {
        return SSD1306_CMD_FAILED;
    }

    if (SSD1306_SetDisplayStartLine(obj, obj->startLine) < 0) {
        return SSD1306_CMD_FAILED;
    }

    if (SSD1306_SetSegmentRemap(obj, obj->segmentMap) < 0) {
        return SSD1306_CMD_FAILED;
    }

    if (SSD1306_SetMultiplexRatio(obj, obj->multiplexRatio) < 0) {
        return SSD1306_CMD_FAILED;
    }

    if (SSD1306_SetScanDirection(obj, obj->scanDirection) < 0) {
        return SSD1306_CMD_FAILED;
    }

    if (SSD1306_SetDisplayOffset(obj, obj->displayOffset) < 0) {
        return SSD1306_CMD_FAILED;
    }

    if (SSD1306_SetPinSequenceConfig(obj, obj->pinSequence) < 0) {
        return SSD1306_CMD_FAILED;
    }

    if (SSD1306_SetPinDirectionConfig(obj, obj->pinDirection) < 0) {
        return SSD1306_CMD_FAILED;
    }

    if (SSD1306_SetClock(obj, obj->clockFreq) < 0) {
        return SSD1306_CMD_FAILED;
    }

    if (SSD1306_SetClockDiv(obj, obj->clockDiv) < 0) {
        return SSD1306_CMD_FAILED;
    }

    if (SSD1306_SetPreChargePhasePeriod1(obj, obj->phasePeriod1) < 0) {
        return SSD1306_CMD_FAILED;
    }

    if (SSD1306_SetPreChargePhasePeriod2(obj, obj->phasePeriod2) < 0) {
        return SSD1306_CMD_FAILED;
    }

    if (SSD1306_SetVoltageDeselectLevel(obj, obj->deselectLevel) < 0) {
        return SSD1306_CMD_FAILED;
    }
    
    if (SSD1306_EnableChargePump(obj, obj->chargePumpEnabled) < 0) {
        return SSD1306_CMD_FAILED;
    }

    if (SSD1306_SetDisplayState(obj, ds) < 0) {
        return SSD1306_CMD_FAILED;
    }

    obj->isInitialized = true;
    return SSD1306_SUCCESS;
}

int32_t SSD1306_DeInit(SSD1306_Object_t* obj) {
    obj->isInitialized = false;
    return SSD1306_SUCCESS;
}

int32_t SSD1306_SetContrast(SSD1306_Object_t* obj, const uint8_t contrast) {
    if (ssd1306_fundamental_set_contrast(&(obj->ctx), contrast) < 0) {
        return SSD1306_CMD_FAILED;
    }

    obj->content = contrast;
    return SSD1306_SUCCESS;
}

int32_t SSD1306_SetDisplayContent(SSD1306_Object_t* obj, const SSD1306_DISPLAY_CONTENT_t content) {
    if (ssd1306_fundamental_set_entire_display_on(&(obj->ctx), content) < 0) {
        return SSD1306_CMD_FAILED;
    }

    obj->content = content;
    return SSD1306_SUCCESS;
}

int32_t SSD1306_SetDisplayColorInverse(SSD1306_Object_t* obj, const SSD1306_DISPLAY_COLOR_t color) {
    if (ssd1306_fundamental_set_display_normal_or_inverse(&(obj->ctx), color) < 0) {
        return SSD1306_CMD_FAILED;
    }

    obj->color = color;
    return SSD1306_SUCCESS;
}

int32_t SSD1306_SetDisplayState(SSD1306_Object_t* obj, const SSD1306_DISPLAY_STATE_t displayState) {
    if (ssd1306_fundamental_set_display_on(&(obj->ctx), displayState) < 0) {        
        return SSD1306_CMD_FAILED;
    }

    obj->displayState = displayState;
    return SSD1306_SUCCESS;
}

int32_t SSD1306_SetVScrollConfig(SSD1306_Object_t* obj, const SSD1306_VerticalScrollSetup_t* config) {
    if (ssd1306_scrolling_continuous_vertical_scroll_setup(&(obj->ctx), config->direction, config->startPage, config->endPage, config->interval, config->offset) < 0) {
        return SSD1306_CMD_FAILED;
    }

    obj->vScrollConfig = *config;
    return SSD1306_SUCCESS;
}

int32_t SSD1306_SetVHScrollConfig(SSD1306_Object_t* obj, const SSD1306_HorizontalScrollSetup_t* config) {
    if (ssd1306_scrolling_continuous_horizontal_scroll_setup(&(obj->ctx), config->direction, config->startPage, config->endPage, config->interval) < 0) {
        return SSD1306_CMD_FAILED;
    }

    obj->hScrollConfig = *config;
    return SSD1306_SUCCESS;
}

int32_t SSD1306_ActivateScrolling(SSD1306_Object_t* obj) {
    if (ssd1306_scrolling_activate(&(obj->ctx)) < 0) {
        return SSD1306_CMD_FAILED;
    }

    obj->isScrollActive = true;
    return SSD1306_SUCCESS;
}

int32_t SSD1306_DeactivateScrolling(SSD1306_Object_t* obj) {
    if (ssd1306_scrolling_deactivate(&(obj->ctx)) < 0) {
        return SSD1306_CMD_FAILED;
    }

    obj->isScrollActive = false;
    return SSD1306_SUCCESS;
}

int32_t SSD1306_SetVScrollArea(SSD1306_Object_t* obj, const uint8_t topFixed, const uint8_t rows) {
    if (ssd1306_scrolling_set_scroll_vertical_scroll_area(&(obj->ctx), topFixed, rows) < 0) {
        return SSD1306_CMD_FAILED;
    }
    return SSD1306_SUCCESS;
}

int32_t SSD1306_SetColumnStartPageAddress(SSD1306_Object_t* obj, const uint8_t startAddress) {
    if ((ssd1306_addressing_set_lower_column_start_address_for_page_addressing_mode(&(obj->ctx), (SSD1306_CMD_ADDRESSING_LOWER_NIBBLE_MASK & startAddress)) |
         ssd1306_addressing_set_higher_column_start_address_for_page_addressing_mode(&(obj->ctx), (SSD1306_CMD_ADDRESSING_HIGHER_NIBBLE_MASK & startAddress))) < 0) {
        return SSD1306_CMD_FAILED;
    }

    return SSD1306_SUCCESS;
}

int32_t SSD1306_SetColumnAddress(SSD1306_Object_t* obj, const uint8_t startAddress, const uint8_t endAddress) {
    if (ssd1306_addressing_set_column_address(&(obj->ctx), startAddress, endAddress) < 0) {
        return SSD1306_CMD_FAILED;
    }

    return SSD1306_SUCCESS;
}

int32_t SSD1306_SetPageAddressForHVMode(SSD1306_Object_t* obj, const uint8_t startAddress, const uint8_t endAddress) {
    if (ssd1306_addressing_set_page_address_for_horizotal_vertical_addressing_mode(&(obj->ctx), startAddress, endAddress) < 0) {
        return SSD1306_CMD_FAILED;
    }

    return SSD1306_SUCCESS;
}

int32_t SSD1306_SetPageStartForPageMode(SSD1306_Object_t* obj, const uint8_t startAddress) {
    if (ssd1306_addressing_set_page_address_for_page_addressing_mode(&(obj->ctx), startAddress) < 0) {
        return SSD1306_CMD_FAILED;
    }

    return SSD1306_SUCCESS;
}

int32_t SSD1306_SetDisplayStartLine(SSD1306_Object_t* obj, const uint8_t startLine) {
    uint8_t sl = startLine & SSD1306_DISPLAY_START_LINE_MASK;

    if (ssd1306_hw_config_set_display_start_line(&(obj->ctx), sl) < 0) {
        return SSD1306_CMD_FAILED;
    }

    obj->startLine = sl;
    return SSD1306_SUCCESS;
}

int32_t SSD1306_SetSegmentRemap(SSD1306_Object_t* obj, const SSD1306_DISPLAY_SEGMENT_MAP_t segmentMap) {
    if (ssd1306_hw_config_set_segment_remap(&(obj->ctx), segmentMap) < 0) {
        return SSD1306_CMD_FAILED;
    }

    obj->segmentMap = segmentMap;
    return SSD1306_SUCCESS;
}

int32_t SSD1306_SetMultiplexRatio(SSD1306_Object_t* obj, const uint8_t multiplexRatio) {
    uint8_t mr = multiplexRatio & SSD1306_DISPLAY_MULTIPLEX_RATIO_MASK;

    if (mr < SSD1306_DISPLAY_MULTIPLEX_RATIO_MIN) {
        mr = SSD1306_DISPLAY_MULTIPLEX_RATIO_MIN;
    }

    if (ssd1306_hw_config_set_multiplex_ratio(&(obj->ctx), mr) < 0) {
        return SSD1306_CMD_FAILED;
    }

    obj->multiplexRatio = mr;
    return SSD1306_SUCCESS;
}

int32_t SSD1306_SetScanDirection(SSD1306_Object_t* obj, const SSD1306_DISPLAY_SCAN_DIRECTION_t scanDirection) {
    if (ssd1306_hw_config_set_com_output_scan_direction(&(obj->ctx), scanDirection) < 0) {
        return SSD1306_CMD_FAILED;
    }

    obj->scanDirection = scanDirection;
    return SSD1306_SUCCESS;
}

int32_t SSD1306_SetDisplayOffset(SSD1306_Object_t* obj, const uint8_t displayOffset) {
    uint8_t doff = displayOffset & SSD1306_DISPLAY_OFFSET_MASK;

    if (ssd1306_hw_config_set_display_offset(&(obj->ctx), doff) < 0) {
        return SSD1306_CMD_FAILED;
    }

    obj->displayOffset = doff;
    return SSD1306_SUCCESS;
}

int32_t SSD1306_SetPinSequenceConfig(SSD1306_Object_t* obj, const SSD1306_DISPLAY_PIN_SEQUENCE_t pinSequence) {
    uint8_t pinConfig;

    pinConfig = (obj->pinDirection << SSD1306_DISPLAY_PIN_DIRECTION_BIT_SHIFT) | pinSequence;
    if (ssd1306_hw_config_set_com_pin_config(&(obj->ctx), pinConfig) < 0) {
        return SSD1306_CMD_FAILED;
    }

    obj->pinSequence = pinSequence;
    return SSD1306_SUCCESS;
}

int32_t SSD1306_SetPinDirectionConfig(SSD1306_Object_t* obj, const SSD1306_DISPLAY_PIN_DIRECTION_t pinDirection) {
    uint8_t pinConfig;

    pinConfig = (pinDirection << SSD1306_DISPLAY_PIN_DIRECTION_BIT_SHIFT) | obj->pinSequence;
    if (ssd1306_hw_config_set_com_pin_config(&(obj->ctx), pinConfig) < 0) {
        return SSD1306_CMD_FAILED;
    }

    obj->pinDirection = pinDirection;
    return SSD1306_SUCCESS;
}

int32_t SSD1306_SetClock(SSD1306_Object_t* obj, const uint8_t clockFreq) {
    uint8_t cf = clockFreq & SSD1306_DISPLAY_CLOCK_MASK;

    if (ssd1306_timing_and_driving_scheme_set_display_clock_div_clock_freq(&(obj->ctx), obj->clockDiv, cf) < 0) {
        return SSD1306_CMD_FAILED;
    }

    obj->clockFreq = cf;
    return SSD1306_SUCCESS;
}

int32_t SSD1306_SetClockDiv(SSD1306_Object_t* obj, const uint8_t clockDiv) {
    uint8_t cd = clockDiv & SSD1306_DISPLAY_CLOCK_DIV_MASK;

    if (ssd1306_timing_and_driving_scheme_set_display_clock_div_clock_freq(&(obj->ctx), cd, obj->clockFreq) < 0) {
        return SSD1306_CMD_FAILED;
    }

    obj->clockDiv = cd;
    return SSD1306_SUCCESS;
}

int32_t SSD1306_SetPreChargePhasePeriod1(SSD1306_Object_t* obj, const uint8_t period) {
    uint8_t pp = period & SSD1306_DISPLAY_PHASE_PERIOD_MASK;

    /* 0x00 is invalid */
    if (!pp) {
        pp++;
    }

    if (ssd1306_timing_and_driving_scheme_set_pre_charge_period(&(obj->ctx), pp, obj->phasePeriod2) < 0) {
        return SSD1306_CMD_FAILED;
    }

    obj->phasePeriod1 = pp;
    return SSD1306_SUCCESS;
}

int32_t SSD1306_SetPreChargePhasePeriod2(SSD1306_Object_t* obj, const uint8_t period) {
    uint8_t pp = period & SSD1306_DISPLAY_PHASE_PERIOD_MASK;

    /* 0x00 is invalid */
    if (!pp) {
        pp++;
    }

    if (ssd1306_timing_and_driving_scheme_set_pre_charge_period(&(obj->ctx), obj->phasePeriod1, pp) < 0) {
        return SSD1306_CMD_FAILED;
    }

    obj->phasePeriod2 = pp;
    return SSD1306_SUCCESS;
}

int32_t SSD1306_SetVoltageDeselectLevel(SSD1306_Object_t* obj, const SSD1306_DISPLAY_DESELECT_LEVEL_t deselectLevel) {
    if (ssd1306_timing_and_driving_scheme_set_vcomh_deselect_level(&(obj->ctx), deselectLevel) < 0) {
        return SSD1306_CMD_FAILED;
    }

    obj->deselectLevel = deselectLevel;
    return SSD1306_SUCCESS;
}

int32_t SSD1306_NOP(SSD1306_Object_t* obj) {
    return ssd1306_timing_and_driving_scheme_nop(&(obj->ctx));
}

int32_t SSD1306_EnableChargePump(SSD1306_Object_t* obj, const bool enable) {
    if (ssd1306_charge_pump_set_charge_pump(&(obj->ctx), enable) < 0) {
        return SSD1306_CMD_FAILED;
    }

    obj->chargePumpEnabled = enable;
    return SSD1306_SUCCESS;
}

int32_t SSD1306_RegisterIO(SSD1306_Object_t* obj, SSD1306_IO_t* io) {
    memcpy(&(obj->io), io, sizeof(*io));
    obj->ctx.WriteReg = SSD1306_WriteRegWrap;
    obj->ctx.ReadReg = SSD1306_ReadReagWrap;
    obj->ctx.handle = obj;

    if (obj->io.Init) {
        return obj->io.Init();
    }
    return SSD1306_SUCCESS;
}

int32_t SSD1306_ReadID(SSD1306_Object_t* obj, uint32_t* id) {
    (void)obj;
    *id = 0xFFFFFFFFU;
    return SSD1306_SUCCESS;
}