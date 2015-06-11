#define F_CPU		8000000
#define BAUD		9600
#define DEV_NAME	"BatteryController1.0"

#define CFG_AUTO_ON					0
#define CFG_OFF_BY_BUTTON			2
#define CFG_WATCHDOG_TIME			600

#define BATT_CELLS					4
#define BATT_VOLT_DISCHARGEOFF		2800	*BATT_CELLS
#define BATT_VOLT_DISCHARGEON		2900	*BATT_CELLS
#define BATT_VOLT_CHARGEON			4000	*BATT_CELLS
#define BATT_VOLT_CHARGEOFF			4100	*BATT_CELLS

#define ADC_VOLT_MULTIPLIER_MV		0.01
#define ADC_CURRENT_MULTIPLIER_MA	10.0

#define ADC_ADJ ADC_ADJUST_RIGHT
#define ADC_REF ADC_REFS_INTERNAL
#define ADC_CLK ADC_PRESCALER_128

#include <avr/io.h>
#include <util/setbaud.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <stdint.h>
#include "libs/bwl_uart.h"
#include "libs/bwl_simplserial.h"
#include "libs/bwl_adc.h"
#include "libs/bwl_tools.h"