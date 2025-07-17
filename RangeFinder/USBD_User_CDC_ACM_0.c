/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::USB:Device:CDC
 * Copyright (c) 2004-2020 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    USBD_User_CDC_ACM_0.c
 * Purpose: USB Device Communication Device Class (CDC)
 *          Abstract Control Model (ACM) User module
 * Rev.:    V6.4.1
 *----------------------------------------------------------------------------*/
/**
 * \addtogroup usbd_cdcFunctions
 *
 * USBD_User_CDC_ACM_0.c implements the application specific functionality
 * of the CDC ACM class and is used to receive and send data to the USB Host.
 *
 * The implementation must match the configuration file USBD_Config_CDC_0.h.
 * The following values in USBD_Config_CDC_0.h affect the user code:
 *
 *  - 'Maximum Communication Device Send Buffer Size' specifies the maximum
 *    value for \em len in \ref USBD_CDC_ACM_WriteData
 *
 *  - 'Maximum Communication Device Receive Buffer Size' specifies the maximum
 *    value for \em len in \ref USBD_CDC_ACM_ReadData
 *
 */
 
 
//! [code_USBD_User_CDC_ACM]
 
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
 
#include "rl_usb.h"
#include "cmsis_os2.h"

/* API prototypes to handle commands */
#define MAX_CMD_SIZE (4U) /* find better solution */
#define DEFAULT_PORT (0U)
#define RX_BUF_SIZE  (256U)
#define CMD_RECEIVED_FLAG (1U << 0U)

typedef struct {
  uint8_t type;
  uint8_t cmd_size;
  uint8_t data[MAX_CMD_SIZE];
} DISPLAY_DRIVER_CMD_t;

extern int32_t display_driver_get_cmd_len(uint8_t cmd_type);
extern int32_t display_driver_populate_cmd_struct(uint8_t cmd_type, uint8_t* data, DISPLAY_DRIVER_CMD_t* cmd);
extern int32_t display_driver_exec_cmd(DISPLAY_DRIVER_CMD_t* cmd);
extern osEventFlagsId_t usbd_ef;
 
// Local Variables
static   CDC_LINE_CODING        cdc_acm_line_coding = { 0U, 0U, 0U, 0U };
static uint8_t cdc_rx_buf[RX_BUF_SIZE];
uint8_t cmd[RX_BUF_SIZE];
uint8_t rx_count;
static uint16_t rx_size;
 
// Called during USBD_Initialize to initialize the USB CDC class instance (ACM).
void USBD_CDC0_ACM_Initialize (void) {
  // Add code for initialization
}
 
 
// Called during USBD_Uninitialize to de-initialize the USB CDC class instance (ACM).
void USBD_CDC0_ACM_Uninitialize (void) {
  // Add code for de-initialization
}
 
 
// Called upon USB Bus Reset Event.
void USBD_CDC0_ACM_Reset (void) {
  // Add code for reset
}
 
 
// Callback function called upon reception of request send encapsulated command sent by the USB Host.
// \param[in]   buf           buffer that contains send encapsulated command request.
// \param[in]   len           length of send encapsulated command request.
// \return      true          send encapsulated command request processed.
// \return      false         send encapsulated command request not supported or not processed.
bool USBD_CDC0_ACM_SendEncapsulatedCommand (const uint8_t *buf, uint16_t len) {
  (void)buf;
  (void)len;

  return true;
}
 
 
// Callback function called upon reception of request to get encapsulated response sent by the USB Host.
// \param[in]   max_len       maximum number of data bytes that USB Host expects to receive
// \param[out]  buf           pointer to buffer containing get encapsulated response to be returned to USB Host.
// \param[out]  len           pointer to number of data bytes to be returned to USB Host.
// \return      true          get encapsulated response request processed.
// \return      false         get encapsulated response request not supported or not processed.
bool USBD_CDC0_ACM_GetEncapsulatedResponse (uint16_t max_len, uint8_t **buf, uint16_t *len) {
  (void)max_len;
  (void)buf;
  (void)len;

  return true;
}
 
 
// Called upon USB Host request to change communication settings.
// \param[in]   line_coding   pointer to CDC_LINE_CODING structure.
// \return      true          set line coding request processed.
// \return      false         set line coding request not supported or not processed.
bool USBD_CDC0_ACM_SetLineCoding (const CDC_LINE_CODING *line_coding) {
  // Add code for set line coding
 
  // Store requested settings to local variable
  cdc_acm_line_coding = *line_coding;
 
  return true;
}
 
 
// Called upon USB Host request to retrieve communication settings.
// \param[out]  line_coding   pointer to CDC_LINE_CODING structure.
// \return      true          get line coding request processed.
// \return      false         get line coding request not supported or not processed.
bool USBD_CDC0_ACM_GetLineCoding (CDC_LINE_CODING *line_coding) {
 
  // Load settings from ones stored on USBD_CDC0_ACM_SetLineCoding callback
  *line_coding = cdc_acm_line_coding;
 
  return true;
}
 
 
// Called upon USB Host request to set control line states.
// \param [in]  state         control line settings bitmap.
//                - bit 0: DTR state
//                - bit 1: RTS state
// \return      true          set control line state request processed.
// \return      false         set control line state request not supported or not processed.
bool USBD_CDC0_ACM_SetControlLineState (uint16_t state) {
  // Add code for set control line state
 
  (void)(state);
 
  return true;
}
 

typedef enum {
  IDLE = 0,
  RECEIVING,
} ACM_DATA_STATE_t;
static ACM_DATA_STATE_t state = IDLE;

// Called when new data was received.
// \param [in]  len           number of bytes available for reading.
void USBD_CDC0_ACM_DataReceived (uint32_t len) {
  // Add code for handling new data reception
  int i = 0;
  int char_data;
  switch (state) {
    case IDLE:
      memset(cdc_rx_buf, 0, sizeof(cdc_rx_buf));
      rx_count = 0;
      rx_size = RX_BUF_SIZE;
      state = RECEIVING;
    case RECEIVING:
      if (len > rx_size) {
        /* buffer overflow */
        state = IDLE;
        return;
      }

      for (; i < len; i++) {
        char_data = USBD_CDC_ACM_GetChar(DEFAULT_PORT);
        if (char_data < 0) {
          /* Error getting data, back to IDLE */
          state = IDLE;
          return;
        }

        /* Echo received char to VCOM */
        USBD_CDC_ACM_PutChar(DEFAULT_PORT, char_data);

        if ((char)char_data == '\r') { /* Carriege Return (Enter Key)*/
          USBD_CDC_ACM_PutChar(DEFAULT_PORT, (int)'\n');
          memcpy(cmd, cdc_rx_buf, rx_count);
          state = IDLE;
          /* notify cmd received */
          (void)osEventFlagsSet(usbd_ef, CMD_RECEIVED_FLAG);
          return;
        }

        cdc_rx_buf[rx_count] = (uint8_t)((char)char_data);
        rx_count++;
        rx_size--;
      }
    default:
      break;
  }
}
 
 
// Called when when all data was sent.
void USBD_CDC0_ACM_DataSent (void) {
  // Add code for handling new data send
}

//! [code_USBD_User_CDC_ACM]
