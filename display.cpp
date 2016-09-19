#include "main.h"
// period is microseconds / 10
// How long to wait inbetween displaying all the rows
#define VBLANK_PERIOD 30

// How long to display each row for
#define DISPLAY_PERIOD 10

#include <TimerOne.h>

unsigned char *disp0;
unsigned char *disp;
unsigned char disp1[256];
unsigned char disp2[256];

void init_display() {
	randomize_display();

	Timer1.initialize(100);
	Timer1.attachInterrupt(updateDisplay, 100);
}

void randomize_display() {
	disp = disp1;
	for (int i = 0; i < 224; i++)
		disp1[i] = disp2[i] = rand() & 0xFF;
}

void clear_display(unsigned char *disp_in) {
	for (int i = 0; i < 224; i++)
		disp_in[i] = 0;
}

boolean displaying = false;
byte disp_ctr = 0;
volatile byte disp_row = 0;

// We actually set the interrupt here, and comment out the handler in
// Timer1.h, just to speed things up a tiny amount
void updateDisplay() {
}
ISR(TIMER1_OVF_vect) {

	// We wait a bit after displaying each row
	if (displaying) {
		disp_ctr++;
		if (disp_ctr < DISPLAY_PERIOD)
			return;

		disp_ctr = 0;

		// Disable the 3-to-8 decoders output
		digitalWrite(eBothPin, LOW);
		digitalWrite(eFirstPin, HIGH);

		// Reset the shift registers (probably not needed, but can't hurt)
		digitalWrite(rstPin, HIGH);
		digitalWrite(rstPin, LOW);

		displaying = false;
	}

	// After displaying the last row, we wait a bit. Time determined by VBLANK_PERIOD
	if (disp_row == 16) {
		disp_ctr++;
		if (disp_ctr >= VBLANK_PERIOD) {
			disp_ctr = 0;
			disp_row = 0;
		} else {
			return;
		}
	} else {
		disp_row++;
	}

	// Shift 14 bits out to each of the 8 logical shift register chains
	for (byte c = 0; c < 14; c++) {
		PORTB = disp[c + 14 * disp_row];
		digitalWrite(clkPin, LOW);
		digitalWrite(clkPin, HIGH);
	}

	// Select the correct row via 3-to-8 decoders (rows 0-7 and 8-15 overlap)
	for (byte i = 0; i < 3; i++) {
		if ((disp_row & 7) & (1 << i))
			digitalWrite(rPins[i], HIGH);
		else
			digitalWrite(rPins[i], LOW);
	}

	// Enable either the first or second set of decoders
	digitalWrite(eBothPin, disp_row > 7);
	digitalWrite(eFirstPin, LOW);
	displaying = true;
}

int xx = 0;
int yy = 0;
//    PORTB = disp[c+7*r];

// Return the state of a given pixel in the main display buffer
boolean getpx(int x, int y) {
	if (x < 0 || y < 0 || x > 111 || y > 15)
		return 0;
	return disp[14 * y + (x / 8)] & (1 << (x & 7));
}

// Set the state of a given pixel in the next display buffer
void setpx(int x, int y, boolean p) {
	if (x < 0 || y < 0 || x > 111 || y > 15)
		return;
	if (p)
		disp0[14 * y + (x / 8)] |= 1 << (x & 7);
	else
		disp0[14 * y + (x / 8)] &= ~(1 << (x & 7));
}

