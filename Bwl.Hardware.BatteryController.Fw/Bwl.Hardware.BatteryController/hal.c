void sserial_send_start(){};
void sserial_send_end(){};
	
	int adc_read_avg()
	{
		int count=2;
		unsigned long sum=0;
		for (int i=0; i<count; i++)
		{
			sum+=adc_read_once();
		}
		return sum>>1;
	}

uint16_t get_voltage_input()
{
	adc_init(ADC_MUX_ADC6,ADC_ADJ,ADC_REF,ADC_CLK);
	uint16_t val=adc_read_avg();
	uint16_t result=val*ADC_VOLT_MULTIPLIER_MV;
	return result;
}

uint16_t get_voltage_battery()
{
	adc_init(ADC_MUX_ADC7,ADC_ADJ,ADC_REF,ADC_CLK);
	uint16_t val=adc_read_avg();
	uint16_t result=val*ADC_VOLT_MULTIPLIER_MV;
	return result;	
}

uint16_t get_voltage_ard()
{
	adc_init(ADC_MUX_ADC4,ADC_ADJ,ADC_REF,ADC_CLK);
	uint16_t val=adc_read_avg();
	uint16_t result=val*ADC_VOLT_MULTIPLIER_MV;
	return result;
}

uint16_t get_current_out()
{
	adc_init(ADC_MUX_ADC5,ADC_ADJ,ADC_REF,ADC_CLK);
	uint16_t val=adc_read_avg();
	uint16_t result=val*ADC_CURRENT_MULTIPLIER_MA;
	return result;	
}

byte get_button_1()
{
	bit_set(PORTD,5);
	bit_clr(DDRD,5);	
	if bit_read(PIND,5) {return 0;}
	return 1;
}

byte get_button_2()
{
	bit_set(PORTD,6);
	bit_clr(DDRD,6);
	if bit_read(PIND,6) {return 0;}
	return 1;	
}

byte _discharge=0;
byte _charge=0;

byte get_discharge()
{
	return _discharge;
}

byte get_charge()
{
	return _charge;
}

void led(byte r, byte g, byte b)
{
	bit_set(DDRD,2);
	bit_set(DDRD,3);
	bit_set(DDRD,4);	
	if (r){bit_set(PORTD,2);}else{bit_clr(PORTD,2);}
	if (g){bit_set(PORTD,3);}else{bit_clr(PORTD,3);}
	if (b){bit_set(PORTD,4);}else{bit_clr(PORTD,4);}	
}

void ledi(byte b)
{
		bit_set(DDRB,0);
		if (b){bit_set(PORTB,0);}else{bit_clr(PORTB,0);}
}

void set_battery_charge(byte state)
{
	_charge=state;
	bit_set(DDRC,1);
	if (state){bit_set(PORTC,1);}else{bit_clr(PORTC,1);}
}

void set_battery_discharge(byte state)
{
	_discharge=state;
	bit_set(DDRC,0);
	if (state){bit_set(PORTC,0);}else{bit_clr(PORTC,0);}
}