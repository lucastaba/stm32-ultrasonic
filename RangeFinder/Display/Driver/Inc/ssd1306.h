#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>
#include <stdbool.h>

#include "ssd1306_reg.h"

/* Types */
typedef enum {
    FOLLOW_RAM = 0,
    ALL_ON,
} SSD1306_DISPLAY_CONTENT_t;

typedef enum {
    NORMAL,
    INVERSE,
} SSD1306_DISPLAY_COLOR_t;

typedef enum {
    SLEEP = 0,
    ON,
} SSD1306_DISPLAY_STATE_t;

typedef enum {
    SEG0_TO_0 = 0,
    SEG0_TO_127,
} SSD1306_DISPLAY_SEGMENT_MAP_t;

typedef enum {
    COM0_TO_N,
    COMN_TO_0,
} SSD1306_DISPLAY_SCAN_DIRECTION_t;

typedef enum {
    SEQUENTIAL,
    ALTERNATIVE,
} SSD1306_DISPLAY_PIN_SEQUENCE_t;

typedef enum {
    RIGHT_TO_LEFT,
    LEFT_TO_RIGHT,
} SSD1306_DISPLAY_PIN_DIRECTION_t;

typedef enum {
    DESELECT_LEVEL_1 = 0x00,
    DESELECT_LEVEL_2 = 0x02,
    DESELECT_LEVEL_3 = 0x03,
} SSD1306_DISPLAY_DESELECT_LEVEL_t;

/* IO API */
typedef int32_t (*SSD1306_IO_Init_Fptr)(void);
typedef int32_t (*SSD1306_IO_DeInit_Fptr)(void);
typedef int32_t (*SSD1306_IO_WriteReg_Fptr)(const uint8_t addr, const uint8_t reg, const uint8_t* data, const uint16_t len);
typedef int32_t (*SSD1306_IO_ReadReg_Fptr)(const uint8_t addr, const uint8_t reg, uint8_t* data, const uint16_t len);

typedef struct {
    SSD1306_IO_Init_Fptr Init;
    SSD1306_IO_DeInit_Fptr DeInit;
    SSD1306_IO_WriteReg_Fptr WriteReg;
    SSD1306_IO_ReadReg_Fptr ReadReg;
    uint8_t i2cAddress;
} SSD1306_IO_t;

/* Object */
typedef struct {
    SSD1306_IO_t io;
    ssd1306_ctx_t ctx;

    bool isInitialized;
    uint8_t contrast;
    SSD1306_DISPLAY_CONTENT_t content;
    SSD1306_DISPLAY_COLOR_t color;
    SSD1306_DISPLAY_STATE_t displayState;
    uint8_t startLine;
    SSD1306_DISPLAY_SEGMENT_MAP_t segmentMap;
    uint8_t multiplexRatio;
    SSD1306_DISPLAY_SCAN_DIRECTION_t scanDirection;
    uint8_t displayOffset;
    SSD1306_DISPLAY_PIN_SEQUENCE_t pinSequence;
    SSD1306_DISPLAY_PIN_DIRECTION_t pinDirection;
    uint8_t clockFreq;
    uint8_t clockDiv;
    uint8_t phasePeriod1;
    uint8_t phasePeriod2;
    SSD1306_DISPLAY_DESELECT_LEVEL_t deselectLevel;
    bool chargePumpEnabled;
} SSD1306_Object_t;

/* Most Common Class Driver API */
typedef int32_t (*SSD11306_Drv_Init_Fptr)(SSD1306_Object_t* obj);
typedef int32_t (*SSD11306_Drv_DeInit_Fptr)(SSD1306_Object_t* obj);

typedef struct {
    SSD11306_Drv_Init_Fptr Init;
    SSD11306_Drv_DeInit_Fptr DeInit;
} SSD1306_Drv_t;

/* Driver API */
int32_t SSD1306_Init(SSD1306_Object_t* obj);
int32_t SSD1306_DeInit(SSD1306_Object_t* obj);
int32_t SSD1306_SetContrast(SSD1306_Object_t* obj, const uint8_t contrast);
int32_t SSD1306_SetDisplayContent(SSD1306_Object_t* obj, const SSD1306_DISPLAY_CONTENT_t content);
int32_t SSD1306_SetDisplayColorInverse(SSD1306_Object_t* obj, const SSD1306_DISPLAY_COLOR_t color);
int32_t SSD1306_SetDisplayState(SSD1306_Object_t* obj, const SSD1306_DISPLAY_STATE_t displayState);
int32_t SSD1306_SetDisplayStartLine(SSD1306_Object_t* obj, const uint8_t startLine);
int32_t SSD1306_SetSegmentRemap(SSD1306_Object_t* obj, const SSD1306_DISPLAY_SEGMENT_MAP_t segmentMap);
int32_t SSD1306_SetMultiplexRatio(SSD1306_Object_t* obj, const uint8_t multiplexRatio);
int32_t SSD1306_SetScanDirection(SSD1306_Object_t* obj, const SSD1306_DISPLAY_SCAN_DIRECTION_t scanDirection);
int32_t SSD1306_SetDisplayOffset(SSD1306_Object_t* obj, const uint8_t displayOffset);
int32_t SSD1306_SetPinSequenceConfig(SSD1306_Object_t* obj, const SSD1306_DISPLAY_PIN_SEQUENCE_t pinSequence);
int32_t SSD1306_SetPinDirectionConfig(SSD1306_Object_t* obj, const SSD1306_DISPLAY_PIN_DIRECTION_t pinDirection);
int32_t SSD1306_SetClock(SSD1306_Object_t* obj, const uint8_t clockFreq);
int32_t SSD1306_SetClockDiv(SSD1306_Object_t* obj, const uint8_t clockDiv);
int32_t SSD1306_SetPreChargePhasePeriod1(SSD1306_Object_t* obj, const uint8_t period);
int32_t SSD1306_SetPreChargePhasePeriod2(SSD1306_Object_t* obj, const uint8_t period);
int32_t SSD1306_SetVoltageDeselectLevel(SSD1306_Object_t* obj, const SSD1306_DISPLAY_DESELECT_LEVEL_t deselectLevel);
int32_t SSD1306_NOP(SSD1306_Object_t* obj);
int32_t SSD1306_EnableChargePump(SSD1306_Object_t* obj, const bool enable);

int32_t SSD1306_RegisterIO(SSD1306_Object_t* obj, SSD1306_IO_t* io);
int32_t SSD1306_ReadID(SSD1306_Object_t* obj, uint32_t* id);

#endif /* SSD1306_H */