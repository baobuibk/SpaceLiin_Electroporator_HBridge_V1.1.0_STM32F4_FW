/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include <string.h>
#include <stdlib.h>

#include "app.h"

#include "fsp_frame.h"
#include "crc.h"
//#include "pwm.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//extern Accel_Gyro_DataTypedef _gyro, _accel;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static void fsp_print(uint8_t packet_length);
//static void convertTemperature(float temp, uint8_t buf[]);
//static void convertIntegerToBytes(int number, uint8_t arr[]);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*
extern double   compensated_pressure;
extern double   compensated_temperature;

float           temp;
uint32_t        press;
*/

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
uint8_t hs_relay_pole, ls_relay_pole, relay_state;
void FSP_Line_Process()
{
switch (ps_GPC_FSP->CMD)
{

/* :::::::::: Pulse Control Command :::::::: */
case FSP_CMD_SET_PULSE_COUNT:
{
	hv_pulse_pos_count 	= ps_GPC_FSP->Payload.set_pulse_count.HV_pos_count;
	hv_pulse_neg_count 	= ps_GPC_FSP->Payload.set_pulse_count.HV_neg_count;

	lv_pulse_pos_count 	= ps_GPC_FSP->Payload.set_pulse_count.LV_pos_count;
	lv_pulse_neg_count 	= ps_GPC_FSP->Payload.set_pulse_count.LV_neg_count;

	UART_Send_String(&RS232_UART, "Received FSP_CMD_PULSE_COUNT\r\n> ");
	break;
}
	

case FSP_CMD_SET_PULSE_DELAY:
{
	hv_delay_ms = ps_GPC_FSP->Payload.set_pulse_delay.HV_delay;
	lv_delay_ms	= ps_GPC_FSP->Payload.set_pulse_delay.LV_delay;

	pulse_delay_ms = ps_GPC_FSP->Payload.set_pulse_delay.Delay_high;
	pulse_delay_ms <<= 8;
	pulse_delay_ms |= ps_GPC_FSP->Payload.set_pulse_delay.Delay_low;

	UART_Send_String(&RS232_UART, "Received FSP_CMD_PULSE_DELAY\r\n> ");
	break;
}
	

case FSP_CMD_SET_PULSE_HV:
{
	hv_on_time_ms 	= ps_GPC_FSP->Payload.set_pulse_HV.OnTime;
	hv_off_time_ms 	= ps_GPC_FSP->Payload.set_pulse_HV.OffTime;

	UART_Send_String(&RS232_UART, "Received FSP_CMD_PULSE_HV\r\n> ");
	break;
}
	
case FSP_CMD_SET_PULSE_LV:
{
	lv_on_time_ms 	= ps_GPC_FSP->Payload.set_pulse_LV.OnTime_high;
	lv_on_time_ms   <<= 8;
	lv_on_time_ms	|= ps_GPC_FSP->Payload.set_pulse_LV.OnTime_low;

	lv_off_time_ms	= ps_GPC_FSP->Payload.set_pulse_LV.OffTime_high;
	lv_off_time_ms	<<= 8;
	lv_off_time_ms	|= ps_GPC_FSP->Payload.set_pulse_LV.OffTime_low;

	UART_Send_String(&RS232_UART, "Received FSP_CMD_PULSE_LV\r\n> ");
	break;
}
	
case FSP_CMD_SET_PULSE_CONTROL:
{
	H_Bridge_Set_Pole();
	is_h_bridge_enable = ps_GPC_FSP->Payload.set_pulse_control.State;
	SchedulerTaskEnable(0, 1);

	UART_Send_String(&RS232_UART, "Received FSP_CMD_PULSE_CONTROL\r\n> ");
	break;
}
	

case FSP_CMD_HANDSHAKE:
{
	ps_GPP_FSP->CMD = FSP_CMD_HANDSHAKE;
	ps_GPP_FSP->Payload.handshake.Check = 0xAB;
	
	fsp_print(2);
	
	UART_Send_String(&RS232_UART, "Received FSP_CMD_HANDSHAKE\r\n> ");
	break;
}
	
default:
	break;
}
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

static void fsp_print(uint8_t packet_length)
{
	s_GPP_FSP_Packet.sod 		= FSP_PKT_SOD;
	s_GPP_FSP_Packet.src_adr 	= fsp_my_adr;
	s_GPP_FSP_Packet.dst_adr 	= FSP_ADR_GPC;
	s_GPP_FSP_Packet.length 	= packet_length;
	s_GPP_FSP_Packet.type 		= FSP_PKT_TYPE_CMD_W_DATA;
	s_GPP_FSP_Packet.eof 		= FSP_PKT_EOF;
	s_GPP_FSP_Packet.crc16 		= crc16_CCITT(FSP_CRC16_INITIAL_VALUE, &s_GPP_FSP_Packet.src_adr, s_GPP_FSP_Packet.length + 4);

	uint8_t encoded_frame[100] = { 0 };
	uint8_t frame_len;
	fsp_encode(&s_GPP_FSP_Packet, encoded_frame, &frame_len);

	UART_FSP(&GPC_UART, (char*)encoded_frame, frame_len);
}

/*
static void convertTemperature(float temp, uint8_t buf[]) {
	// temperature is xxx.x format
	//float to byte

	gcvt(temp, 5, buf);
}

static void convertIntegerToBytes(int number, uint8_t arr[]) {
	arr[0]= number & 0xff;
	arr[1]= (number >>8 ) & 0xff;
	arr[2] = (number >>16) & 0xff;
	arr[3] = (number >>24) & 0xff;
}
*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */