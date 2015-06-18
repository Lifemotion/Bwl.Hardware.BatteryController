#include "config.h"
#include "hal.c"


void sserial_process_request()
{
	if (sserial_request.command==10)
	{
		uint16_t  volt_int0= get_voltage_ard();
		uint16_t  volt_bat=get_voltage_battery();
		uint16_t  volt_in= get_voltage_input();
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
		sserial_response.data[20]=0;
		sserial_response.data[21]=volt_int0>>8;
		sserial_response.data[22]=volt_int0&255;
		sserial_response.datalength=23;
		sserial_send_response();
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
	
	/*while(1)
	{
	set_battery_discharge(get_button_1());
	set_battery_charge(get_button_2());
	led(get_button_1(),get_button_2(),0);
	sserial_poll_uart();
	wdt_reset();
	}*/
	
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
	set_battery_discharge(0);
	if (CFG_AUTO_ON) {set_battery_discharge(1);}
	while(1)
	{
		uint16_t vbatt=	get_voltage_battery();
		uint16_t vin=	get_voltage_input();
		uint16_t vard=	get_voltage_ard();
		//uint16_t curr=	get_current_out();
		
		byte vin_present=	(vin>BATT_VOLT_CHARGEOFF);		 //???
		//byte batt_empty=	(vbatt<BATT_VOLT_DISCHARGEOFF);
		byte batt_low=		(vbatt<BATT_VOLT_DISCHARGEON);
		byte batt_full=		(vbatt>BATT_VOLT_CHARGEON);
		byte discharge=		get_discharge();
		byte charge=		get_charge();
		
		if (vbatt<BATT_VOLT_DISCHARGEOFF)
		{
			set_battery_charge(1);
			//батарея разряжена, мигаем красным
			for (int i=0; i<20; i++)
			{
				led(255,0,0);_delay_ms(500);led(0,0,0);	_delay_ms(500);
			}
			if (vbatt<BATT_VOLT_DISCHARGEOFF)
			{
				//все еще разряжена, отключаем разряд
				set_battery_charge(0);
				set_battery_discharge(0);
				//при этом контроллер тоже выключится
			}
		}
		
		if (vin_present)
		{
			//входное напряжение есть
			if (vbatt<BATT_VOLT_CHARGEON) {set_battery_charge(1);}
			if (vbatt>BATT_VOLT_CHARGEOFF){set_battery_charge(0);}
		}else
		{
			set_battery_charge(0);
		}
		
		if (vard>BATT_VOLT_DISCHARGEOFF)
		{
			//нажата кнопка
			if (discharge)
			{
				if (CFG_OFF_BY_BUTTON)	set_battery_discharge(0);
			}else
			{
				set_battery_discharge(1);
			}
			wdt_reset();
			_delay_ms(1000);
		}
		
		if (CFG_LEDS)
		{
			if (vin_present)
			{
				if (charge){led(1,discharge,0);}else{led(0,discharge,1);}
			}
			else
			{led(charge,discharge,0);}
				
			if (get_button_1())
			{
				for (int i=0; i<5; i++)
				{
					led(batt_low,batt_full,1);_delay_ms(200);led(0,0,0);	_delay_ms(200);
				}
			}
		}
		
		sserial_poll_uart();
		wdt_reset();
	}
}