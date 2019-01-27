/*
 * main.c
 *
 * Created: 07.01.2019 08:17:27
 * Authors : Fabian Diga, Stefan Naujoks, Andrian Grubba
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "LCD_student.h"
#include "peripheries.h"
#include "interface.h"
#include "settings.h"
#define F_CPU 16000000UL

#define TRUE 1
#define FALSE 0

/************************************************************************/
/* Enumerable that holds different states of the software               */
/************************************************************************/
enum {
	init,
	checkSensor1,
	checkSensor2,
	errorSensor,
	waitForResetGlobal,
	waitForErrorReset,
	initSensor1,
	startMeasure,
	initSensor2,
	stopMeasure,
	printTime
};
// Current state
int nextState = init;

// System runtime in milliseconds
unsigned volatile long long int runningTimeMs = 0;

// Tracks a waiting duration in milliseconds
unsigned int waitTimeMs = 0;

// Timestamp that holds the start of 
// the wait time in milliseconds
unsigned int startwaitTimeMs = 0;

// Time between sensor1 trigger and
// sensor2 trigger in milliseconds
long int measuredTime = 0;

// Tracks the reset button press time in milliseconds
long int buttonPressTimeMs = 0;

// States that hold the trigger state
// of the two sensors
unsigned volatile char sensor1state = FALSE;
unsigned volatile char sensor2state = FALSE;


int main(void)
{
    while (1) 
    {		
		/************************************************************************/
		/* When the reset button is pressed TIME_UNTIL_RESET_MS					*/
		/* milliseconds the software goes back to init state					*/
		/* @see settings.h														*/
		/************************************************************************/
		if(buttonPressTimeMs >= TIME_UNTIL_RESET_MS)
		{
			nextState = init;
			buttonPressTimeMs = 0;
		}
		
		// Main state machine
		switch(nextState)
		{
			/************************************************************************/
			/* Setup of IO, LCD, Timer, Interrupts									*/
			/* Reset of all running variables										*/                                           
			/************************************************************************/
			case init:
				sensor1state = FALSE;
				sensor2state = FALSE;
				
				runningTimeMs = 0;
				waitTimeMs = 0;
				startwaitTimeMs = 0;
				measuredTime = 0;
				buttonPressTimeMs = 0;
					
				setLS1Interrupt(FALSE);
				setLS2Interrupt(FALSE);
				setupButton();
				setupLichtschranke1();
				setupLichtschranke2();
				LCD_init();
				setupTimer0();
				sei();
				nextState = checkSensor1;	
			break;
			
			/************************************************************************/
			/* Check functionality of sensor 1                                      */
			/* If sensor 1 is not triggered											*/
			/* within TIME_UNITL_SENSOR_ERROR_MS milliseconds						*/
			/* the LCD prints an error												*/
			/* @see settings.h														*/
			/************************************************************************/
			case checkSensor1:
				LCD_clear_screan();
				set_Curser_position(3,0);
				LCD_print_String("Interrupt");
				set_Curser_position(3,1);
				LCD_print_String("sensor 1");
			
				sensor1state = FALSE;
				setLS1Interrupt(TRUE);
				
				runningTimeMs = 0;
				while(!sensor1state)
				{
					if(runningTimeMs >=TIME_UNITL_SENSOR_ERROR_MS)
					{
						break;
					}
				}
				if(runningTimeMs >=TIME_UNITL_SENSOR_ERROR_MS)
				{
					nextState = errorSensor;
				}
				else
				{
					nextState = checkSensor2;
				}	
				
				setLS1Interrupt(FALSE);
				sensor1state = FALSE;
				
			break;
			
			/************************************************************************/
			/* Check functionality of sensor 2                                      */
			/* If sensor 2 is not triggered											*/
			/* within TIME_UNITL_SENSOR_ERROR_MS milliseconds						*/
			/* the LCD prints an error												*/
			/* @see settings.h														*/
			/************************************************************************/
			case checkSensor2:
				LCD_clear_screan();
				set_Curser_position(3,0);
				LCD_print_String("Interrupt");
				set_Curser_position(3,1);
				LCD_print_String("sensor 2");
				
				sensor2state = FALSE;
				setLS2Interrupt(TRUE);
				
				
				runningTimeMs = 0;
				while(!sensor2state)
				{
					if(runningTimeMs >=TIME_UNITL_SENSOR_ERROR_MS)
					{
						break;
					}
				}
				if(runningTimeMs >=TIME_UNITL_SENSOR_ERROR_MS)
				{
					nextState = errorSensor;
				}
				else
				{
					nextState = initSensor1;
				}
				
				setLS2Interrupt(FALSE);
				sensor2state = FALSE;
				
			break;
			
			/************************************************************************/
			/* If a sensor is not triggered	while in check state                    */
			/* an error is printed on the LCD										*/
			/************************************************************************/
			case errorSensor:
				LCD_clear_screan();
				set_Curser_position(3,0);
				LCD_print_String("Error");
				set_Curser_position(3,1);
				LCD_print_String("sensor");
				
				nextState = waitForErrorReset;
				
			break;
			
			/************************************************************************/
			/* After a sensor error the reset button                                */
			/* has to be pressed to start a new sensor check						*/
			/************************************************************************/
			case waitForErrorReset:
				while((BUTTON1_READ & (1<<BUTTON1_MASK))!=(1<<BUTTON1_MASK)){}
				if((BUTTON1_READ & (1<<BUTTON1_MASK))==(1<<BUTTON1_MASK)){
					nextState = checkSensor1;
				}
			break;
			
			/************************************************************************/
			/* Activate sensor 1 / deactivate sensor 2                              */
			/* LCD prints a message that the measurement is ready					*/
			/************************************************************************/	
			case initSensor1:
		
				setLS1Interrupt(TRUE);
				setLS2Interrupt(FALSE);
				sensor1state = FALSE;
				sensor2state = FALSE;
				
				LCD_clear_screan();
				set_Curser_position(3,0);
				LCD_print_String("Ready to");
				set_Curser_position(3,1);
				LCD_print_String("measure");
				
				nextState = startMeasure;
				
			break;

			/************************************************************************/
			/* Deactivate sensor 1 / activate sensor 2                              */
			/************************************************************************/
			case initSensor2:
				setLS1Interrupt(FALSE);
				setLS2Interrupt(TRUE);
				sensor1state = FALSE;
				sensor2state = FALSE;
				
				nextState = stopMeasure;
			break;
			
			/************************************************************************/
			/* When sensor 1 is triggered the measurement starts                    */
			/* A countdown for TIME_UNTIL_SENSOR1_WAIT_TIME_RESET_MS				*/
			/* milliseconds starts													*/
			/* If sensor 1 is triggered within this time, the countdown				*/
			/* starts again															*/
			/* @see settings.h														*/
			/************************************************************************/
			case startMeasure:
				while(!sensor1state){}
				runningTimeMs = 0;
				sensor1state = FALSE;
				
				LCD_clear_screan();
				set_Curser_position(0,0);
				LCD_print_String("Measuring time..");
				
				waitTimeMs = 0;
				while(waitTimeMs <=TIME_UNTIL_SENSOR1_WAIT_TIME_RESET_MS)
				{
					waitTimeMs = 0;
					startwaitTimeMs = runningTimeMs;
					
					while(!sensor1state && waitTimeMs<=TIME_UNTIL_SENSOR1_WAIT_TIME_RESET_MS)
					{
						waitTimeMs = runningTimeMs-startwaitTimeMs;
					}
					sensor1state = FALSE;
				}
				
				nextState = initSensor2;			
			break;
			
			/************************************************************************/
			/* When sensor 2 is triggered the measurement stops                     */
			/* All sensor interrupts are deactivated								*/
			/************************************************************************/
			case stopMeasure:
				if(sensor2state)
				{
					measuredTime = runningTimeMs;
					setLS1Interrupt(FALSE);
					setLS2Interrupt(FALSE);
					sensor2state = FALSE;
					sensor1state = FALSE;
					nextState = printTime;
				}		
			break;

			/************************************************************************/
			/* After the measurement is done the measured time                      */
			/* is formatted into: mm:ss:mmm											*/
			/************************************************************************/
			case printTime:
				LCD_clear_screan();
				set_Curser_position(0,0);
				LCD_print_String("Measured time:");
				set_Curser_position(2,1);
				
				char buffer[17];
				int min = measuredTime/60000;
				measuredTime -= min*60000;

				int secs = measuredTime/1000;
				measuredTime -= secs*1000; 

				int milli = measuredTime;
				
				sprintf(buffer,"%02i:%02i.%03i",min,secs,milli);
				LCD_print_String(buffer);
				
				nextState = waitForResetGlobal;
				
			break;
			
			/************************************************************************/
			/* After a successful measurement the reset button                      */
			/* has to be pressed to start a new measurement							*/
			/************************************************************************/
			case waitForResetGlobal:
			while((BUTTON1_READ & (1<<BUTTON1_MASK))!=(1<<BUTTON1_MASK)){}
			if((BUTTON1_READ & (1<<BUTTON1_MASK))==(1<<BUTTON1_MASK)){
				nextState = initSensor1;
			}
			
			break;
		}
    }
}

//Timer 0 compare A interrupt
ISR(TIMER0_COMPA_vect)
{
	runningTimeMs++;
	if((BUTTON1_READ & (1<<BUTTON1_MASK))==(1<<BUTTON1_MASK))
	{
		buttonPressTimeMs++;
	}
	else
	{
		buttonPressTimeMs = 0;
	}
}

//Lichtschranke 1 interrupt
ISR(INT0_vect)
{
	sensor1state = TRUE;
}

//Lichtschranke 2 interrupt
ISR(INT1_vect)
{
	sensor2state = TRUE;
}
