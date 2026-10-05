/*
 * wizchip_port.h
 *
 *  Created on: Oct 27, 2025
 *      Author: arunrawat
 */

#ifndef INC_WIZCHIP_PORT_H_
#define INC_WIZCHIP_PORT_H_

#include <stdint.h>
#include <stdbool.h>

int W5500_Init(void);
int  TCP_Client_Connect(void);
int  TCP_Client_Send(uint8_t *data, uint16_t len);

extern volatile bool tcp_connected;

#endif /* INC_WIZCHIP_PORT_H_ */
