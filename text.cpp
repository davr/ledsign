/*
 * text.cpp
 *
 *  Created on: Mar 25, 2012
 *      Author: davidr
 */

#include "main.h"
#include "SystemFont5x7.h"

void setup_txt() {

}
volatile int x=0,y=0;
void loop_txt() {
	x+=7;
	if(x >= 112) {
		x-= 112;
		y++;
		if(y>15) {
			y=0;
		}
	}
	setpx(x,y,1);

	while (disp_row != 16)
		;
	disp = disp1;
	disp0 = disp2;
}


