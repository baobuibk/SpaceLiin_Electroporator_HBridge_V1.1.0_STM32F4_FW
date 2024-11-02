#ifndef _FSP_FRAME_H

#define _FSP_FRAME_H

// Commands
typedef enum _FSP_CMD_typedef_
{
	/* :::::::::: Pulse Control Command :::::::: */
	FSP_CMD_SET_PULSE_POLE,
	FSP_CMD_SET_PULSE_COUNT,
	FSP_CMD_SET_PULSE_DELAY,
	FSP_CMD_SET_PULSE_HV,
	FSP_CMD_SET_PULSE_LV,
	FSP_CMD_SET_PULSE_CONTROL,

	/* :::::::::: VOM Command :::::::: */
	FSP_CMD_MEASURE_CURRENT,
	FSP_CMD_MEASURE_IMPEDANCE,

	/* :::::::::: I2C Sensor Command :::::::: */
	FSP_CMD_GET_BMP390,
	FSP_CMD_GET_LMSDOX,

	/* :::::::::: Ultility Command :::::::: */
	FSP_CMD_HANDSHAKE,
	
} FSP_CMD_typedef;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Pulse Control Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
typedef struct _FSP_SET_PULSE_POLE_FRAME_
{
	uint8_t 	pos_pole;		/* hv pulse count */
	uint8_t 	neg_pole;		/* hv pulse count */

} FSP_SET_PULSE_POLE_FRAME;

typedef struct _FSP_SET_PULSE_COUNT_FRAME_
{
	uint8_t 	HV_pos_count;		/* hv pulse count */
	uint8_t 	HV_neg_count;		/* hv pulse count */
	uint8_t 	LV_pos_count;		/* lv pulse count */
	uint8_t 	LV_neg_count;		/* lv pulse count */

} FSP_SET_PULSE_COUNT_FRAME;

typedef struct _FSP_SET_PULSE_DELAY_FRAME_
{
	uint8_t 	HV_delay;
	uint8_t 	LV_delay;
	uint8_t 	Delay_high;		  	/* Delay time */
	uint8_t 	Delay_low;		  	/* Delay time */

} FSP_SET_PULSE_DELAY_FRAME;

typedef struct _FSP_SET_PULSE_HV_FRAME_
{
	uint8_t 	OnTime;      	/* HV On time */
	uint8_t 	OffTime;      	/* HV Off time */

} FSP_SET_PULSE_HV_FRAME;

typedef struct _FSP_SET_PULSE_LV_FRAME_
{
	uint8_t 	OnTime_high;      	/* LV On time */
	uint8_t 	OnTime_low;      	/* LV On time */
	uint8_t 	OffTime_high;      	/* LV Off time */
	uint8_t 	OffTime_low;      	/* LV Off time */

} FSP_SET_PULSE_LV_FRAME;

typedef struct _FSP_SET_PULSE_CONTROL_FRAME_
{
	uint8_t 	State;      	/* 0: OFF, 1: ON */

} FSP_SET_PULSE_CONTROL_FRAME;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ VOM Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
typedef struct _MEASURE_CURRENT_FRAME_
{
	uint8_t 	Value_high;
	uint8_t 	Value_low;

} MEASURE_CURRENT_FRAME;

typedef struct _MEASURE_IMPEDANCE_FRAME_
{
	uint8_t		Period;
	uint8_t 	Value_high;
	uint8_t 	Value_low;

} MEASURE_IMPEDANCE_FRAME;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ I2C Sensor Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
typedef struct _GET_BMP390_
{
	uint8_t 	temp[5];
	uint8_t 	pressure[6];

} GET_BMP390;

typedef struct _GET_LSMDOX_
{
	uint8_t 	accel_x[4];
	uint8_t 	accel_y[4];
	uint8_t 	accel_z[4];
	uint8_t 	gyro_x[4];
	uint8_t 	gyro_y[4];
	uint8_t 	gyro_z[4];

} GET_LSMDOX;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ultility Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
typedef struct _FSP_HANDSAKE_
{
	uint8_t 	Check;

} FSP_HANDSAKE;

// Union to encapsulate all frame types
typedef union _FSP_Payload_Frame_typedef_
{
	/* :::::::::: Pulse Control Command :::::::: */
	FSP_SET_PULSE_POLE_FRAME				set_pulse_pole;
	FSP_SET_PULSE_COUNT_FRAME				set_pulse_count;
	FSP_SET_PULSE_DELAY_FRAME				set_pulse_delay;
	FSP_SET_PULSE_HV_FRAME					set_pulse_HV;
	FSP_SET_PULSE_LV_FRAME					set_pulse_LV;
	FSP_SET_PULSE_CONTROL_FRAME				set_pulse_control;

	/* :::::::::: VOM Command :::::::: */
	MEASURE_CURRENT_FRAME					measure_current;
	MEASURE_IMPEDANCE_FRAME					measure_impedance;

	/* :::::::::: I2C Sensor Command :::::::: */
	GET_BMP390								get_BMP390;
	GET_LSMDOX								get_LSMDOX;

	/* :::::::::: Ultility Command :::::::: */
	FSP_HANDSAKE							handshake;

} FSP_Payload_Frame_typedef;

typedef struct _FSP_Payload_typedef_
{
	uint8_t						CMD;
	FSP_Payload_Frame_typedef 	Payload;
	
} FSP_Payload;

/* :::::::::: FSP Line Process ::::::::::::: */
void FSP_Line_Process();

#endif
