void sserial_send_start(){};
void sserial_send_end(){};
	
uint16_t get_voltage_input()
{
	adc_init(ADC_MUX_ADC2,ADC_ADJ,ADC_REF,ADC_CLK);
	uint16_t val=adc_read_average(100);
	uint16_t result=val*ADC_VOLT_MULTIPLIER_MV;
	return result;
}

uint16_t get_voltage_battery()
{
	adc_init(ADC_MUX_ADC3,ADC_ADJ,ADC_REF,ADC_CLK);
	uint16_t val=adc_read_average(100);
	uint16_t result=val*ADC_VOLT_MULTIPLIER_MV;
	return result;	
}

uint16_t get_current_out()
{
	adc_init(ADC_MUX_ADC2,ADC_ADJ,ADC_REF,ADC_CLK);
	uint16_t val=adc_read_average(100);
	uint16_t result=val*ADC_CURRENT_MULTIPLIER_MA;
	return result;	
}

byte get_button_1()
{
	bit_set(PORTB,1);
	bit_clr(DDRB,1);	
	if bit_read(PINB,1) {return 0;}
	return 1;
}

byte get_button_2()
{
	bit_set(PORTB,1);
	bit_clr(DDRB,1);
	if bit_read(PINB,1) {return 0;}
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
	bit_set(DDRB,1);
	bit_set(DDRB,1);
	bit_set(DDRB,1);	
	if (r){bit_set(PORTB,1);}else{bit_clr(PORTB,1);}
	if (g){bit_set(PORTB,1);}else{bit_clr(PORTB,1);}
	if (b){bit_set(PORTB,1);}else{bit_clr(PORTB,1);}	
}

void set_battery_charge(byte state)
{
	_charge=state;
	//bit_set(DDRB,1);
	//if (state){bit_set(PORTB,1);}else{bit_clr(PORTB,1);}
}

void set_battery_discharge(byte state)
{
	_discharge=state;
	bit_set(DDRB,1);
	if (state){bit_set(PORTB,1);}else{bit_clr(PORTB,1);}
}