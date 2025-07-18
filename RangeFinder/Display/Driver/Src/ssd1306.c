#include <string.h>

#include "ssd1306.h"
#include "ssd1306_reg.h"

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

int32_t SSD1306_SetContrast(SSD1306_Object_t* obj, const uint8_t contrast) {
    return ssd1306_fundamental_set_contrast(&(obj->ctx), contrast);
}

int32_t SSD1306_SetDisplayContent(SSD1306_Object_t* obj, const SSD1306_DISPLAY_CONTENT_t content) {
    return ssd1306_fundamental_set_entire_display_on(&(obj->ctx), content == FOLLOW_RAM);
}

int32_t SSD1306_SetDisplayColorInverse(SSD1306_Object_t* obj, const SSD1306_DISPLAY_COLOR_t color) {
    return ssd1306_fundamental_set_display_normal_or_inverse(&(obj->ctx), color == INVERSE);
}

int32_t SSD1306_DisplayON(SSD1306_Object_t* obj, const SSD1306_DISPLAY_STATE_t state) {
    return ssd1306_fundamental_set_display_on(&(obj->ctx), state == ON);
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