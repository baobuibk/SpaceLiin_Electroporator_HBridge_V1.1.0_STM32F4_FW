/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include <stdlib.h>

#include "stm32f4xx_ll_gpio.h"

#include "app.h"
#include "command.h"

#include "cmd_line_task.h"
#include "h_bridge_task.h"

#include "cmd_line.h"
//#include "pwm.h"
//#include "fsp.h"
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
//extern uart_stdio_typedef  RS232_UART;
//extern uart_stdio_typedef  GPP_UART;

//extern Accel_Gyro_DataTypedef _gyro, _accel;
//extern PWM_TypeDef H_Bridge_1_PWM;
//extern PWM_TypeDef H_Bridge_2_PWM;

tCmdLineEntry g_psCmdTable[] =
{
    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Pulse Control Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "SET_PULSE_POLE",			CMD_SET_PULSE_POLE, 		" : Set pole for H Bridge Pole" },
    { "SET_PULSE_COUNT",		CMD_SET_PULSE_COUNT, 		" : Set number of pulse" },
    { "SET_PULSE_DELAY",		CMD_SET_PULSE_DELAY, 		" : Set delay between pulse hv and lv" },
    { "SET_PULSE_HV", 			CMD_SET_PULSE_HV, 			" : Set hs pulse on time and off time" },
    { "SET_PULSE_LV", 			CMD_SET_PULSE_LV, 			" : Set ls pulse on time and off time" },
    { "SET_PULSE_CONTROL", 		CMD_SET_PULSE_CONTROL, 		" : Start pulsing" },

	{ "GET_PULSE_POLE",			CMD_GET_PULSE_POLE, 		" : Set pole for H Bridge Pole" },
	{ "GET_PULSE_COUNT",		CMD_GET_PULSE_COUNT, 		" : Get number of pulse" },
	{ "GET_PULSE_DELAY",		CMD_GET_PULSE_DELAY, 		" : Get delay between pulse hv and lv" },
	{ "GET_PULSE_HV", 			CMD_GET_PULSE_HV, 			" : Get hs pulse on time and off time" },
	{ "GET_PULSE_LV", 			CMD_GET_PULSE_LV, 			" : Get ls pulse on time and off time" },
	{ "GET_PULSE_CONTROL", 		CMD_GET_PULSE_CONTROL, 		" : Get info whether pulse starting pulsing" },
	{ "GET_PULSE_ALL", 			CMD_GET_PULSE_ALL, 			" : Get all info about pulse" },

    /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ultility Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
    { "MARCO",                  CMD_LINE_TEST,              "TEST" },
	{0,0,0}
};
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* :::::::::: Pulse Control Command :::::::: */
int CMD_SET_PULSE_POLE(int argc, char *argv[])
{
	if (argc < 3)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 3)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm[2];

	receive_argm[0] = atoi(argv[1]);
	receive_argm[1] = atoi(argv[2]);

	if (receive_argm[0] == receive_argm[1])
		return CMDLINE_INVALID_ARG;
	else if ((receive_argm[0] > 8) || (receive_argm[0] < 1) || (receive_argm[0] == 9))
		return CMDLINE_INVALID_ARG;
	else if ((receive_argm[1] > 8) || (receive_argm[1] < 1) || (receive_argm[1] == 9))
		return CMDLINE_INVALID_ARG;

	HB_pos_pole_index = receive_argm[0] - 1;
	HB_neg_pole_index = receive_argm[1] - 1;

	H_Bridge_Set_Pole();

	return CMDLINE_OK;
}

int CMD_SET_PULSE_COUNT(int argc, char *argv[])
{
	if (argc < 5)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 5)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm[4];

	receive_argm[0] = atoi(argv[1]);
	receive_argm[1] = atoi(argv[2]);
	receive_argm[2] = atoi(argv[3]);
	receive_argm[3] = atoi(argv[4]);

	if ((receive_argm[0] > 20) || (receive_argm[1] > 20) || (receive_argm[2] > 20) || (receive_argm[3] > 20))
		return CMDLINE_INVALID_ARG;

	hv_pulse_pos_count 	= receive_argm[0];
    hv_pulse_neg_count 	= receive_argm[1];

    lv_pulse_pos_count 	= receive_argm[2];
    lv_pulse_neg_count 	= receive_argm[3];

	return CMDLINE_OK;
}

int CMD_SET_PULSE_DELAY(int argc, char *argv[])
{
	if (argc < 4)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 4)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm[3];

	receive_argm[0] = atoi(argv[1]);
	receive_argm[1] = atoi(argv[2]);
	receive_argm[2] = atoi(argv[3]);

	if ((receive_argm[0] > 100) || (receive_argm[0] < 0))
		return CMDLINE_INVALID_ARG;
	else if ((receive_argm[1] > 100) || (receive_argm[1] < 0))
		return CMDLINE_INVALID_ARG;
	else if ((receive_argm[2] > 1000) || (receive_argm[2] < 0))
		return CMDLINE_INVALID_ARG;

	hv_delay_ms = receive_argm[0];
	lv_delay_ms	= receive_argm[1];

    pulse_delay_ms = receive_argm[2];

	return CMDLINE_OK;
}

int CMD_SET_PULSE_HV(int argc, char *argv[])
{
	if (argc < 3)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 3)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm[2];

	receive_argm[0] = atoi(argv[1]);
	receive_argm[1] = atoi(argv[2]);

	if ((receive_argm[0] > 20) || (receive_argm[0] < 1))
		return CMDLINE_INVALID_ARG;
	else if ((receive_argm[1] > 20) || (receive_argm[1] < 1))
		return CMDLINE_INVALID_ARG;

	hv_on_time_ms   = receive_argm[0];
	hv_off_time_ms  = receive_argm[1];

	return CMDLINE_OK;
}

int CMD_SET_PULSE_LV(int argc, char *argv[])
{
	if (argc < 3)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 3)
		return CMDLINE_TOO_MANY_ARGS;

	int receive_argm[2];

	receive_argm[0] = atoi(argv[1]);
	receive_argm[1] = atoi(argv[2]);

	if ((receive_argm[0] > 1000) || (receive_argm[0] < 1))
		return CMDLINE_INVALID_ARG;
	else if ((receive_argm[1] > 1000) || (receive_argm[1] < 1))
		return CMDLINE_INVALID_ARG;

	lv_on_time_ms 	= receive_argm[0];
    lv_off_time_ms	= receive_argm[1];

	return CMDLINE_OK;
}

int CMD_SET_PULSE_CONTROL(int argc, char *argv[])
{
	if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;

	int8_t receive_argm = atoi(argv[1]);

	if ((receive_argm > 1) || (receive_argm < 0))
		return CMDLINE_INVALID_ARG;

	is_h_bridge_enable = receive_argm;
	SchedulerTaskEnable(0, 1);

	return CMDLINE_OK;
}

int CMD_GET_PULSE_POLE(int argc, char *argv[])
{
	if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;

	UART_Printf(&RS232_UART, "> PULSE POS POLE: %d; PULSE NEG POLE: %d\n", 
	HB_pos_pole_index + 1, HB_neg_pole_index + 1);

	return CMDLINE_OK;
}

int CMD_GET_PULSE_COUNT(int argc, char *argv[])
{
	if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;

	UART_Printf(&RS232_UART, "> POS HV PULSE COUNT: %d; NEG HV PULSE COUNT: %d\n", 
	hv_pulse_pos_count, hv_pulse_neg_count);

	UART_Printf(&RS232_UART, "> POS LV PULSE COUNT: %d; NEG LV PULSE COUNT: %d\n", 
	lv_pulse_pos_count, lv_pulse_neg_count);

	return CMDLINE_OK;
}

int CMD_GET_PULSE_DELAY(int argc, char *argv[])
{
	if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;

	UART_Printf(&RS232_UART, "> DELAY BETWEEN HV POS AND NEG PULSE: %dms\n", hv_delay_ms);

	UART_Printf(&RS232_UART, "> DELAY BETWEEN LV POS AND NEG PULSE: %dms\n", lv_delay_ms);

	UART_Printf(&RS232_UART, "> DELAY BETWEEN HV PULSE AND LV PULSE: %dms\n", pulse_delay_ms);

	return CMDLINE_OK;		
}

int CMD_GET_PULSE_HV(int argc, char *argv[])
{
	if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;

	UART_Printf(&RS232_UART, "HV PULSE ON TIME: %dms; HV PULSE OFF TIME: %dms\n", 
	hv_on_time_ms, hv_off_time_ms);

	return CMDLINE_OK;
}

int CMD_GET_PULSE_LV(int argc, char *argv[])
{
	if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;

	UART_Printf(&RS232_UART, "> LV PULSE ON TIME: %dms; LV PULSE OFF TIME: %dms\n",
	lv_on_time_ms, lv_off_time_ms);

	return CMDLINE_OK;
}

int CMD_GET_PULSE_CONTROL(int argc, char *argv[])
{
	if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;
	/*
	if (is_h_bridge_enable == 1)
	{
		UART_Send_String(&RS232_UART, "> H BRIDGE IS PULSING\n");
	}
	else
	{
		UART_Send_String(&RS232_UART, "> H BRIDGE IS NOT PULSING\n");
	}
	*/

	UART_Printf(&RS232_UART, "> %s\n", 
	is_h_bridge_enable ? "H BRIDGE IS PULSING" : "H BRIDGE IS NOT PULSING");

	return CMDLINE_OK;
}

int CMD_GET_PULSE_ALL(int argc, char *argv[])
{
	if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;

	UART_Printf(&RS232_UART, "> POS HV PULSE COUNT: %d; NEG HV PULSE COUNT: %d\n", 
	hv_pulse_pos_count, hv_pulse_neg_count);
	UART_Printf(&RS232_UART, "> POS LV PULSE COUNT: %d; NEG LV PULSE COUNT: %d\n", 
	lv_pulse_pos_count, lv_pulse_neg_count);

	UART_Printf(&RS232_UART, "> DELAY BETWEEN HV POS AND NEG PULSE: %dms\n", hv_delay_ms);
	UART_Printf(&RS232_UART, "> DELAY BETWEEN LV POS AND NEG PULSE: %dms\n", lv_delay_ms);
	UART_Printf(&RS232_UART, "> DELAY BETWEEN HV PULSE AND LV PULSE: %dms\n", pulse_delay_ms);

	UART_Printf(&RS232_UART, "HV PULSE ON TIME: %dms; HV PULSE OFF TIME: %dms\n", 
	hv_on_time_ms, hv_off_time_ms);
	UART_Printf(&RS232_UART, "> LV PULSE ON TIME: %dms; LV PULSE OFF TIME: %dms\n",
	lv_on_time_ms, lv_off_time_ms);

	UART_Printf(&RS232_UART, "> %s\n", 
	is_h_bridge_enable ? "H BRIDGE IS PULSING" : "H BRIDGE IS NOT PULSING");

	return CMDLINE_OK;
}

/* :::::::::: Ultility Command :::::::: */
int CMD_LINE_TEST(int argc, char *argv[])
{
    UART_Send_String(&RS232_UART, "> POLO\n");
    return CMDLINE_OK;
}
