/*
 * SignLife.h
 *
 *  Created on: Mar 22, 2012
 *      Author: davidr
 */

#ifndef SIGNLIFE_H_
#define SIGNLIFE_H_

#define MODE_LIFE 1
#define MODE_TETRIS 2
#define MODE_TXT 3

extern int mode;

const int ledPin = 11; // Teensy has LED on 11, Teensy++ on 6

// data pins on the shift registers
const int dPins[] = { 0, 1, 2, 3, 13, 14, 15, 4 };

// row pins on the 3-to-8 decoders
const int rPins[] = { 18, 17, 16 };

// enable pin on both decoders
const int eBothPin = 9;

// enable pin on the first decoder
const int eFirstPin = 10;

// clock pin on shift register
const int clkPin = 21;

// reset pin on shift register
const int rstPin = 20;

// user button
const int btnPin = 19;

#endif /* SIGNLIFE_H_ */
