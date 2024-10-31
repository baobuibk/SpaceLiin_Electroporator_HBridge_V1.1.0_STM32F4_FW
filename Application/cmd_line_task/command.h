#ifndef COMMAND_H_
#define COMMAND_H_

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Include ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include <stdint.h>

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* :::::::::: Pulse Control Command :::::::: */
int CMD_SET_PULSE_POLE(int argc, char *argv[]);
int CMD_SET_PULSE_COUNT(int argc, char *argv[]);
int CMD_SET_PULSE_DELAY(int argc, char *argv[]);
int CMD_SET_PULSE_HV(int argc, char *argv[]);
int CMD_SET_PULSE_LV(int argc, char *argv[]);
int CMD_SET_PULSE_CONTROL(int argc, char *argv[]);

int CMD_GET_PULSE_POLE(int argc, char *argv[]);
int CMD_GET_PULSE_COUNT(int argc, char *argv[]);
int CMD_GET_PULSE_DELAY(int argc, char *argv[]);
int CMD_GET_PULSE_HV(int argc, char *argv[]);
int CMD_GET_PULSE_LV(int argc, char *argv[]);
int CMD_GET_PULSE_CONTROL(int argc, char *argv[]);
int CMD_GET_PULSE_ALL(int argc, char *argv[]);

/* :::::::::: Ultility Command :::::::: */
int CMD_LINE_TEST(int argc, char *argv[]);

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#endif /* COMMAND_H_ */
