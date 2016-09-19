/*
 * main.cpp
 *
 *  Created on: Mar 20, 2012
 *      Author: davidr
 */

#include <WProgram.h>

extern void setup();
extern void loop();

//int main(void) __attribute__((noreturn));
int main(void)
{
	_init_Teensyduino_internal_();

	setup();

	while (1) {
		loop();
	}
}

