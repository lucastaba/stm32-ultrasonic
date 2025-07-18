#ifndef SSD1306_H
#define SSD1306_H

#include <stdint.h>
#include <stdbool.h>

#include "ssd1306_reg.h"

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

typedef struct {
    SSD1306_IO_t io;
    ssd1306_ctx_t ctx;
    bool isInitialized;
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

int32_t SSD1306_RegisterIO(SSD1306_Object_t* obj, SSD1306_IO_t* io);
int32_t SSD1306_ReadID(SSD1306_Object_t* obj, uint32_t* id);

#endif /* SSD1306_H */