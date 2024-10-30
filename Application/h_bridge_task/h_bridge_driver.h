#ifndef H_BRIDGE_DRIVER_H_
#define H_BRIDGE_DRIVER_H_

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Include ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include <stdint.h>
#include <stdbool.h>

#include "stm32f4xx_ll_gpio.h"
#include "pwm.h"
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
typedef enum _H_Bridge_mode_typedef_
{
    H_BRIDGE_MODE_HS_ON,
    H_BRIDGE_MODE_LS_ON,
    H_BRIDGE_MODE_FLOAT,
    H_BRIDGE_MODE_PULSE,
}H_Bridge_mode;

typedef struct _H_Bridge_typdef_
{
    GPIO_TypeDef    *Port;
    uint32_t        Pin;
    bool            Pin_State;
    PWM_TypeDef     *PWM;
    H_Bridge_mode   Mode;
    uint16_t        delay_time_ms;
    uint16_t        on_time_ms;
    uint16_t        off_time_ms;
    uint16_t        pulse_count;
    uint16_t        set_pulse_count;
    bool            is_setted;
}H_Bridge_typdef;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/*
extern H_Bridge_typdef *H_Bridge_array[8];
extern H_Bridge_typdef *p_HB_pos_pole, *p_HB_neg_pole;
*/
extern H_Bridge_typdef *p_HB_pos_pole;
extern H_Bridge_typdef *p_HB_neg_pole;
extern H_Bridge_typdef *p_HB_SD_0_3;
extern H_Bridge_typdef *p_HB_SD_4_7;
extern H_Bridge_typdef H_Bridge_array[8];

extern uint8_t HB_pos_pole_index;
extern uint8_t HB_neg_pole_index;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void H_Bridge_Driver_Init(void);

void H_Bridge_Set_Mode(H_Bridge_typdef* H_Bridge_x, H_Bridge_mode SetMode);

void H_Bridge_Set_Pulse_Timing(H_Bridge_typdef* H_Bridge_x, uint16_t Set_delay_time_ms, uint16_t Set_on_time_ms, uint16_t Set_off_time_ms, uint16_t Set_pulse_count);

void H_Bridge_Kill(void);

void H_Bridge_SD0_3_Interupt_Handle();
void H_Bridge_SD4_7_Interupt_Handle();

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#endif /* H_BRIDGE_DRIVER_H_ */
