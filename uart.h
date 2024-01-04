#ifndef __UART_H__
#define __UART_H__

#include "Arduino.h"
#include "relay.h"

#define UART_MAX_BLOCK_LEN  8
#define MAX_MESSAGE_LEN     64

#define SerialX  Serial

typedef enum
{
    UART_CMD_TRANSMIT_RAW   = 'T',
    UART_CMD_TRANSMIT_NODE  = 'N',
    UART_CMD_GET_AVAIL      = 'A',
    UART_CMD_READ_RAW       = 'R',
    UART_CMD_READ_NODE      = 'O' 
} uart_cmd_et;

typedef enum
{
    UART_REPLY_AVAILABLE    = 'a',
    UART_REPLY_READ_RAW     = 'r',
    UART_REPLY_READ_NODE    = 'o' 
} uart_reply_et;


typedef enum
{
    MSG_FORMAT_RAW = 0,
    MSG_FORMAT_SENSOR_JSON,
    MSG_FORMAT_RELAY_JSON
}  msg_format_et;

typedef enum
{
    STATUS_UNDEFINED = 0,
    STATUS_OK_FOR_ME,
    STATUS_NOT_FOR_ME,
    STATUS_UNKNOWN_COMMAND,
    STATUS_CORRECT_FRAME,
    STATUS_INCORRECT_FRAME,
} msg_status_et;


typedef struct
{
    String          str;
    char            radio_msg[MAX_MESSAGE_LEN];
    uint8_t         len;
    bool            avail;
    char            module;
    char            addr;         
    uart_cmd_et     cmd;
    msg_format_et   format;
    msg_format_et   cmd_format;
    msg_status_et   status;
} uart_rx_st;

typedef struct
{
    String          str;
    uint8_t         len;
    bool            avail;
    char            module;
    char            addr;         
    uart_cmd_et     cmd;
    msg_format_et   format;
    msg_format_et   cmd_format;
    msg_status_et   status;
} uart_tx_st;

typedef struct
{
    String zone;
    String name; 
    String value;
    String remark;
} uart_node_st;

typedef struct
{
    uart_rx_st      rx;
    uart_tx_st      tx;
    uart_node_st    node;
} uart_msg_st;

/// @brief Clear rx available
/// @param  -
/// @return -
void uart_initialize(void);


/// @brief  Send Relay command to radio modem
/// @note   
/// @param  raddr pointer to relay address
/// @param  rcmd  relay on/off/toggle
/// @return -
void uart_send_relay_msg(relay_addr_st *raddr, uart_relay_cmnd_et rcmd);

void uart_send_relay_indx(va_relays_et relay_indx, uart_relay_cmnd_et rcmd);


/// @brief  Read uart
/// @note   Save mesage in uart.rx.str
/// @param  -
/// @return -
void uart_read_uart(void);

/// @brief  Parse Rx frame, 
/// @note   check that the frame is valid and addressed to me
/// @param  -
/// @return -
void uart_parse_rx_frame(void);

/// @brief  Print rx metadat for debugging
/// @param  -
/// @return -
void uart_print_rx_metadata(void);


#endif