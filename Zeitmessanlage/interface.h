/*
 * interface.h
 *
 * Created: 07.01.2019 08:47:34
 * Authors: Fabian Diga, Stefan Naujoks, Andrian Grubba
 *
 * Header for IO configuration
 */ 
#include <avr/io.h>
#ifndef INTERFACE_H_
#define INTERFACE_H_

//Lichtschranke 1
#define LS1_PORT		DDRD
#define LS1_MASK		2
#define LS1_READ		PIND
#define LS1_WRITE		PORTD

//Lichtschranke 2
#define LS2_PORT		DDRD
#define LS2_MASK		3
#define LS2_READ		PIND
#define LS2_WRITE		PORTD

//Button 1 (Reset)
#define BUTTON1_PORT	DDRD
#define BUTTON1_MASK	4
#define BUTTON1_READ	PIND
#define BUTTON1_WRITE	PORTD



#endif /* INTERFACE_H_ */