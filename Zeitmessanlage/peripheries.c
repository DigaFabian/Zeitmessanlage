/*
 * peripheries.c
 *
 * Created: 07.01.2019 08:29:34
 * Authors: Fabian Diga, Stefan Naujoks, Andrian Grubba
 */ 
#include <avr/io.h>
#include "peripheries.h"
#include "interface.h"

/************************************************************************/
/* Setup timer 0                                                        */
/* Set timer 0 to CTC mode												*/
/* Set timer 0 prescaler to  64											*/
/* Set compare value to 249 (250 values) -> 1ms interrupt				*/
/************************************************************************/
 void setupTimer0()
 {
	 //Set timer 0 to CTC mode
	 TCCR0A |= (1<<WGM00);
	 TCCR0A |= (1<<WGM01);
	 TCCR0B &= ~(1<<WGM02);

	 //preload value
	 TCNT0 = 0;

	 //Set prescaler (64 prescaler) -> timer start
	 TCCR0B &= ~(1<<CS02);
	 TCCR0B |= (1<<CS00);
	 TCCR0B |= (1<<CS01);

	 //Set compare value
	 OCR0A = 249;
	 TIMSK0 |= (1<<OCIE0A); 

 }

/************************************************************************/
/* Setup sensor 1                                                      */
/* Set sensor 1 as an input												*/
/* Set Interrupt Sense Control to detect rising edge					*/
/************************************************************************/
 void setupLichtschranke1()
 {
	//Set as input
	LS1_PORT &= ~(1<<LS1_MASK);

	//Set Interrupt Sense Control(ISC) to detect rising edge on INT0
	EICRA |= (1<<ISC01);
	EICRA |= (1<<ISC00);
 }
 
 /************************************************************************/
 /* Function to change the sensor 1 interrupt state                      */
 /************************************************************************/
 void setLS1Interrupt(unsigned char state)
 {
	if(state)
	{
		//Activate INT0
		EIMSK |= (1<<INT0);
	}
	else
	{
		//Disable INT0
		EIMSK &= ~(1<<INT0);
	}
 }


/************************************************************************/
/* Setup sensor 2                                                       */
/* Set sensor 2 as an input												*/
/* Set Interrupt Sense Control to detect rising edge					*/
/************************************************************************/
 void setupLichtschranke2()
 {
	//Set as input
	LS2_PORT &= ~(1<<LS2_MASK);

	//Set Interrupt Sense Control(ISC) to detect rising edge on INT1
	EICRA |= (1<<ISC11);
	EICRA |= (1<<ISC10);
 }

/************************************************************************/
/* Function to change the sensor 2 interrupt state                      */
/************************************************************************/
 void setLS2Interrupt(unsigned char state)
 {
	if(state == 1)
	{
		//Activate INT1
		EIMSK |= (1<<INT1);
	}
	else
	{
		//Disable INT1
		EIMSK &= ~(1<<INT1);
	}
 }

/************************************************************************/
/* Set reset Button as input                                            */
/************************************************************************/
void setupButton()
{
	BUTTON1_PORT &= ~(1<<BUTTON1_MASK);
}
