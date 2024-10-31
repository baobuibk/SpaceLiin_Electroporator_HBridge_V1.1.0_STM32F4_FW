#ifndef APP_H_
#define APP_H_

#include <stdbool.h>
#include <stdint.h>

#include "stm32f411xe.h"

// SYSTEM DRIVER //
#include "board.h"

// USER DRIVER //
#include "scheduler.h"
#include "uart.h"

// INCLUDE TASK //
#include "cmd_line_task.h"
#include "h_bridge_task.h"
#include "v_switch_task.h"
#include "fsp_line_task.h"
/*
#include "adc_task.h"
#include "impedance_task.h"
#include "BMP390.h"
*/

// INCLUDE DRIVER //
#include "h_bridge_driver.h"
#include "v_switch_driver.h"


void App_Main(void);

#endif /* APP_H_ */
