/*
 * main.cpp
 *
 * Created: 10/25/2022 6:20:13 PM
 * Update:	10/27/2022
 *  Author: Christian Gruenewald
 */ 

#include <avr/io.h>	//	all io's defined
#include <avr/interrupt.h>	//	Interupt bib
#define F_CPU 8000000UL	//	8 Mhz clock of mic.controller

const float T0 = 3.0; // T0 = 0.1ms .. 20ms


 // *****************************************************
 // pi-controller
 // using global symbol T0
 // *****************************************************
 inline uint16_t pi_controller(int16_t error)
 {
	 const float VR = 1; // VR = 0.1 .. 10, VR=1-> D=0,7; VR=2->D=0,5; VR=0,5-> D=1 (best value)
	 const float TN = 20.0; // TN = 20ms
	 const float KP_f = VR * 1024;
	 const float KI_f = T0*VR/TN * 1024;
	 const uint32_t KP = KP_f;
	 const uint32_t KI = KI_f;
	 int32_t output, output_i;
	 static int32_t output_i_old = 0;
	 output_i = error * KI + output_i_old;
	 /* limit to positiv value and 20 Bit */
	 if (output_i > 0xFFFFF)
	 output_i = 0xFFFFF;
	 if (output_i < 0)
	 output_i = 0;
	 output_i_old = output_i;
	 output = output_i + error * KP;
	 /* limit to positiv value and 20 Bit */
	 if (output > 0xFFFFF)
	 output = 0xFFFFF;
	 if (output < 0)
	 output = 0;
	 output = output / 1024;
	 return (uint16_t)output;
 }

void init_t1(void)
{
	//	Timer/Counter1: 16 Bit Timer, max val 65535 (16 bit)
	const uint8_t T1_PRESCALER = 8;	//	Prescaler for T1
	const uint16_t T1_TOP = F_CPU/1000000/T1_PRESCALER * (T0*1000);	//	Calculate Top value for T1, 3000
	TCCR1B |= (1<<CS11);	//	Clock/8, set Prescaler 8
	TCCR1B |= (1<<WGM12);	//	Set Timer 1 in CTC mode
	OCR1A = T1_TOP;	//	Load Top value in T1 Top Register
	TIMSK1 |= (1<<OCIE1A);	//	Load Interupt mask, Comp. Match Int. release
	sei();	//	Eneable global Interrupts release
}

void init_t3(void)
{
	OCR3A = 1024;									//	Load Top Value for Timer/Counter3 with 1024
	TCCR3A |= (1<<COM3B1)|(1<<WGM31)|(1<<WGM30);	//	Set Timer3 in FAST-PWM-MODE OCR3A=TOP, //Konfiguriere Output
													//	Compare Port Pin OC3A als nicht invertierend, 1 <<COM3B1
	TCCR3B |= (1<<WGM33)|(1<<WGM32)|(1<<CS30);		//	Set Timer3 in FAST-PWM-MODE OCR3A=TOP. no Prescaler CS30 <<3
	DDRE |= (1<<DDE4);								//	Set PORT E4 as OUTPUT
}

void init_adc(void)
{
	ADCSRA |= (1<<ADEN);			//	Start Analog Digital Converter
	ADMUX |= (1<<REFS0);			//	Set AVCC as ref voltage
	ADMUX |= (1<<MUX1);				//	Set ADC4 pin as input for ADC, enable MUX	
	DIDR0 |= (1<<ADC2D)|(1<<ADC3D);	//		
}


ISR(TIMER1_COMPA_vect)
{
	PORTE|=(1<<PORTE1);	//for testing ISR pull HIGH at Port E1
	
	//	This Interupt runs every 3 ms
	//	set_point (sollwert) on PF3, act_signal (istwert) on PF2
	volatile static uint16_t set_point, act_signal, OUTPUT;
	volatile static int16_t error;
	
	//	measure actual signal
	ADMUX &= ~(1<<MUX0);	//	select 
	ADCSRA |= (1<<ADSC);	//
	while(ADCSRA & (1<<ADSC));	//	Wait until conversion complete
	act_signal = ADCW;	//	Save Conversion 
	//	measure set point
	ADMUX |= (1<<MUX0);	//
	ADCSRA |= (1<<ADSC);	//	Start ADC Conversion
	while(ADCSRA & (1<<ADSC));	//	Wait for conversion ready
	set_point = ADCW;	//	save conversion
	
	//	TODO:: calculate error and implement Controler
	//	calculate and output control signal
	error=set_point-act_signal;
	
	OUTPUT=pi_controller(error);
	
	OCR3B=OUTPUT;	//	PWM output to timer3
	
	
	PORTE&=~(1<<PORTE1);	//	for testing ISR timer ~3ms, ISR runinng 10 us
}


int main(void)
{
	//	Init all Timer and AD-Converter:
	DDRE|=(1<<DDE1); // Port E1 as output 
	init_t1();
	init_t3();
	init_adc();
	
    while(1)
    {
        //	pass, due to ISR controlled 
		
    }
}