/*
 * fsp_line_task.h
 *
 *  Created on: Sep 30, 2024
 *      Author: thanh
 */

#ifndef FSP_LINE_TASK_H_
#define FSP_LINE_TASK_H_

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Include ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "app.h"

#include "fsp.h"
#include "fsp_frame.h"
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
extern uart_stdio_typedef 	GPC_UART;

extern fsp_packet_t		    s_FSP_RX_Packet;
extern fsp_packet_t		    s_FSP_TX_Packet;

extern FSP_Payload  	    *ps_FSP_RX;		//for RX
extern FSP_Payload		    *ps_FSP_TX;		//for TX

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* :::::::::: FSP Line Task Init :::::::: */
void FSP_Line_Task_Init();

/* :::::::::: FSP Line Task ::::::::::::: */
void FSP_Line_Task(void*);

/* :::::::::: IRQ Handler ::::::::::::: */
void GPC_UART_IRQHandler(void);
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#endif /* FSP_LINE_TASK_H_ */

