/*
 * peripheries.h
 *
 * Created: 07.01.2019 08:29:45
 * Authors: Fabian Diga, Stefan Naujoks, Andrian Grubba
 *
 * Header for peripheries.c
 */ 


#ifndef PERIPHERIES_H_
#define PERIPHERIES_H_

void setupTimer0();
void setupLichtschranke1();
void setLS1Interrupt(unsigned char);
void setupLichtschranke2();
void setLS2Interrupt(unsigned char);
void setupButton();




#endif /* PERIPHERIES_H_ */