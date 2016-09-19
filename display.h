/*
 * display.h
 *
 *  Created on: Mar 22, 2012
 *      Author: davidr
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

extern void init_display();
extern void randomize_display();
extern void updateDisplay();
extern boolean getpx(int x, int y);
extern void setpx(int x, int y, boolean p);
extern void clear_display(unsigned char *disp_in);

extern unsigned char *disp0;
extern unsigned char *disp;
extern unsigned char disp1[];
extern unsigned char disp2[];

extern volatile byte disp_row;


#endif /* DISPLAY_H_ */
