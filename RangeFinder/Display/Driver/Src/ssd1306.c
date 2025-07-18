#include <string.h>

#include "ssd1306.h"

#define SSD1306_SUCCESS (0)

static int32_t SSD1306_WriteRegWrap(void* handle, const uint8_t reg, const uint8_t* data , const uint16_t len) {
    SSD1306_Object_t* obj = (SSD1306_Object_t*)(handle);
    return obj->io.WriteReg(obj->io.i2cAddress, reg, data, len);
}
static int32_t SSD1306_ReadReagWrap(void* handle, const uint8_t reg, uint8_t* data, const uint16_t len) {
    SSD1306_Object_t* obj = (SSD1306_Object_t*)(handle);
    return obj->io.ReadReg(obj->io.i2cAddress, reg, data, len);
}

int32_t SSD1306_Init(SSD1306_Object_t* obj) {
    return SSD1306_SUCCESS;
}

int32_t SSD1306_DeInit(SSD1306_Object_t* obj) {
    return SSD1306_SUCCESS;
}

int32_t SSD1306_RegisterIO(SSD1306_Object_t* obj, SSD1306_IO_t* io) {
    memcpy(&(obj->io), io, sizeof(*io));
    obj->ctx.WriteReg = SSD1306_WriteRegWrap;
    obj->ctx.ReadReg = SSD1306_ReadReagWrap;
    obj->ctx.handle = obj;
    
    return SSD1306_SUCCESS;
}

int32_t SSD1306_ReadID(SSD1306_Object_t* obj, uint32_t* id) {
    (void)obj;
    *id = 0xFFFFFFFFU;
    return SSD1306_SUCCESS;
}