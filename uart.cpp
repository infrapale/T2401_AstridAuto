#include "main.h"
#include "relay.h"
#include "uart.h"

//#include "rfm_receive.h"
//#include "rfm_send.h"

extern module_data_st   me;
extern relay_addr_st    relay_addr[VA_RELAY_NBR_OF];

uart_msg_st         uart;

uart_msg_st *uart_get_data_ptr(void)
{
    return &uart;
}

void uart_initialize(void)
{
    uart.rx.avail = false;
}

void uart_read_uart(void)
{
    if (SerialX.available())
    {
        uart.rx.str = SerialX.readStringUntil('\n');
        if (uart.rx.str.length()> 0)
        {
            uart.rx.avail = true;
            uart.rx.str.remove(uart.rx.str.length()-1);
        }
        #ifdef DEBUG_PRINT
        Serial.println("rx is available");
        #endif        
    }
}

void uart_parse_rx_frame(void)
{
    //rfm_send_msg_st *rx_msg = &send_msg; 
    bool do_continue = true;
    uint8_t len;
    uart.rx.str.trim();
    uart.rx.len = uart.rx.str.length();
    if ((uart.rx.str.charAt(0) != '<') || 
        (uart.rx.str.charAt(1) != '#') || 
        (uart.rx.str.charAt(5) != ':') || 
        (uart.rx.str.charAt(uart.rx.len-1) != '>'))  do_continue = false;
    if (do_continue)
    {   
        #ifdef DEBUG_PRINT
        Serial.print("Buffer frame is OK\n");
        #endif

        uart.rx.status = STATUS_CORRECT_FRAME;
        if ((uart.rx.str.charAt(6)  == '{') && 
            (uart.rx.str.charAt(uart.rx.len-2) == '}'))
        {
            uart.rx.format = MSG_FORMAT_SENSOR_JSON;
        } 
        else 
        {
            uart.rx.format = MSG_FORMAT_RAW;
        }   
        uart.rx.module  = uart.rx.str.charAt(2);
        uart.rx.addr    = uart.rx.str.charAt(3);
        uart.rx.cmd     = (uart_cmd_et)uart.rx.str.charAt(4);

        if((uart.rx.module == me.module) && (uart.rx.addr == me.addr))
            uart.rx.status = STATUS_OK_FOR_ME;
        else 
            uart.rx.status = STATUS_NOT_FOR_ME;
    }
    else uart.rx.status = STATUS_INCORRECT_FRAME;
}

void uart_build_node_from_rx_str(void)
{
    uint8_t indx1;
    uint8_t indx2;
    indx1 = 0; 
    indx2 = uart.rx.str.indexOf(';');
    uart.node.zone = uart.rx.str.substring(indx1,indx2);
    indx1 = indx2+1;
    indx2 = uart.rx.str.indexOf(';',indx1+1);
    uart.node.name = uart.rx.str.substring(indx1,indx2);
    indx1 = indx2+1;
    indx2 = uart.rx.str.indexOf(';',indx1+1);
    uart.node.value = uart.rx.str.substring(indx1,indx2);
    indx1 = indx2+1;
    indx2 = uart.rx.str.indexOf(';',indx1+1);
    uart.node.remark = uart.rx.str.substring(indx1,indx2);
    indx1 = indx2+1;
    indx2 = uart.rx.str.indexOf(';',indx1+1);
    
}


void uart_send_relay_msg(relay_addr_st *raddr, uart_relay_cmnd_et rcmd)
{
    uart.tx.str = "<#";
    uart.tx.str += me.module;
    uart.tx.str += me.addr;
    uart.tx.str += (char) UART_CMD_TRANSMIT_NODE;
    uart.tx.str += ':';
    uart.tx.str += raddr->unit;
    uart.tx.str += ';';
    uart.tx.str += raddr->relay;
    uart.tx.str += ';';
    uart.tx.str += rcmd;
    uart.tx.str += ';';
    uart.tx.str += "";
    uart.tx.str += '>';
    Serial.println(uart.tx.str);
}

void uart_send_relay_indx(va_relays_et relay_indx, uart_relay_cmnd_et rcmd)
{
    uart_send_relay_msg( &relay_addr[relay_indx], rcmd);
}

void uart_print_rx_metadata(void)
{
    Serial.print("Length      "); Serial.println(uart.rx.len);
    Serial.print("Avail       "); Serial.println(uart.rx.avail);
    Serial.print("Status      "); Serial.println(uart.rx.status);
    Serial.print("Module      "); Serial.println(uart.rx.module);
    Serial.print("Address     "); Serial.println(uart.rx.addr);
    Serial.print("Command     "); Serial.println(uart.rx.cmd);
    Serial.print("Format      "); Serial.println(uart.rx.format);
}    
