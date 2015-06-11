#include "config.h"
#include "hal.c"


void sserial_process_request()
{
	if (sserial_request.command==10)
	{
	
		uint16_t  volt_in= get_voltage_input();
		uint16_t  volt_bat=get_voltage_battery();
		uint16_t  curr_out=get_current_out();
	
		sserial_response.result=128+sserial_request.command;
		sserial_response.data[0]=get_discharge();//discharge
		sserial_response.data[1]=get_charge();//charge
		sserial_response.data[2]=0;//reserved
		sserial_response.data[3]=0;//reserved
		sserial_response.data[4]=0;//chareservedrge
		sserial_response.data[5]=0;
		sserial_response.data[6]=volt_in>>8;
		sserial_response.data[7]=volt_in&255;
		sserial_response.data[8]=0;
		sserial_response.data[9]=volt_bat>>8;
		sserial_response.data[10]=volt_bat&255;
		sserial_response.data[14]=0;
		sserial_response.data[15]=curr_out>>8;
		sserial_response.data[16]=curr_out&255;		
		sserial_response.datalength=18;
		sserial_send_response();
	}
	if (sserial_request.command==11)
	{
		sserial_response.result=128+sserial_request.command;
		sserial_response.data[0]=0;
		sserial_response.data[1]=BATT_VOLT_DISCHARGEOFF>>8;
		sserial_response.data[2]=BATT_VOLT_DISCHARGEOFF&255;
		sserial_response.data[3]=0;
		sserial_response.data[4]=BATT_VOLT_DISCHARGEON>>8;
		sserial_response.data[5]=BATT_VOLT_DISCHARGEON&255;
		sserial_response.data[6]=0;
		sserial_response.data[7]=BATT_VOLT_CHARGEOFF>>8;
		sserial_response.data[8]=BATT_VOLT_CHARGEOFF&255;
		sserial_response.data[9]=0;
		sserial_response.data[10]=BATT_VOLT_CHARGEON>>8;
		sserial_response.data[11]=BATT_VOLT_CHARGEON&255;
		sserial_response.datalength=12;
		sserial_send_response();
	}
	if (sserial_request.command==20)
	{
		sserial_response.result=128+sserial_request.command;
		sserial_response.data[0]=get_button_1();
		sserial_response.data[1]=get_button_2();
		sserial_response.datalength=2;
		sserial_send_response();
	}	
	if (sserial_request.command==70)
	{
		sserial_response.result=128+sserial_request.command;
		sserial_response.datalength=0;
		led(sserial_request.data[1],sserial_request.data[2],sserial_request.data[3]);
		sserial_send_response();	
	}
	if (sserial_request.command==71)
	{
		sserial_response.result=128+sserial_request.command;
		if ((sserial_request.data[0]==sserial_request.data[2])&&(sserial_request.data[1]==sserial_request.data[3]))
		{
			set_battery_discharge(sserial_request.data[0]);
			set_battery_charge(sserial_request.data[1]);
		}
		sserial_response.datalength=0;
		sserial_send_response();
	}	
}

int main(void)
{
	wdt_enable(WDTO_8S);
	sserial_find_bootloader();
	sserial_setdevname(DEV_NAME);
	uart_init_withdivider(UBRR_VALUE);
	//пауза перед включением
	set_battery_discharge(0);
	set_battery_charge(0);
	get_button_1();
	get_button_2();
	led(0,0,0);
	_delay_ms(500);	
	if (get_voltage_input()>BATT_VOLT_DISCHARGEON)
	{
		//входное напряжение есть
	}else
	{
		//входного напряжения нет
		if (get_voltage_battery()<BATT_VOLT_DISCHARGEON)	
		{
			//напряжение батареи слишком низкое, мигаем красным и ничего не включаем
			while (1)
			{
				led(255,0,0);
				_delay_ms(200);
				led(0,0,0);
				_delay_ms(200);
			}
		}
	}
	//всё нормально, включаем батарею
	set_battery_discharge(1);
	while(1)
	{
		if (get_voltage_battery()<BATT_VOLT_DISCHARGEOFF)
		{
			set_battery_charge(1);
			//батарея разряжена, мигаем красным
			for (int i=0; i<20; i++)
			{
				led(255,0,0);
				_delay_ms(500);
				led(0,0,0);
				_delay_ms(500);				
			}
			if (get_voltage_battery()<BATT_VOLT_DISCHARGEOFF)
			{
				//все еще разряжена, отключаем разряд
				set_battery_charge(0);	
				set_battery_discharge(0);	
				//при этом контроллер тоже выключится	
			}
		}
		if (get_voltage_input()>BATT_VOLT_CHARGEON)
		{
			//входное напряжение есть
			if (get_voltage_battery()<BATT_VOLT_CHARGEON){set_battery_charge(1);}
			if (get_voltage_battery()>BATT_VOLT_CHARGEOFF){set_battery_charge(0);}
		}
		sserial_poll_uart();
		wdt_reset();
	}
}