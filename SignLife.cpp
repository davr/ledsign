#include "main.h"

int mode = MODE_TXT;

void init_pins() {

	for (int i = 0; i < 8; i++) {
		pinMode(dPins[i], OUTPUT);
		digitalWrite(dPins[i], LOW);
	}

	for (int i = 0; i < 3; i++) {
		pinMode(rPins[i], OUTPUT);
		digitalWrite(rPins[i], LOW);
	}

	pinMode(eBothPin, OUTPUT);
	pinMode(eFirstPin, OUTPUT);
	pinMode(clkPin, OUTPUT);
	pinMode(rstPin, OUTPUT);

	pinMode(ledPin, OUTPUT);

	pinMode(btnPin, INPUT);

	// Enable pullup on the button
	digitalWrite(btnPin, HIGH);

	digitalWrite(eBothPin, LOW);
	digitalWrite(eFirstPin, HIGH);

	setup_tetris();
}

long int last_time = 0;

void setup() {
	Serial.begin(9600);

	init_pins();
	init_display();
}

void loop() {
	switch (mode) {
	case MODE_TETRIS:
		loop_tetris();
		break;
	case MODE_LIFE:
		loop_life();
		break;
	case MODE_TXT:
		loop_txt();
		break;
	}

	  while(digitalRead(btnPin)) {
		last_time = -30000;
	    delay(100);
	  }

	if (millis() - last_time > 30000) {
		switch (mode) {
		case MODE_TETRIS:
			mode = MODE_LIFE;
			break;
		case MODE_LIFE:
			mode = MODE_TXT;
			break;
		case MODE_TXT:
			mode = MODE_TETRIS;
			break;
		}
		last_time = millis();
		randomize_display();
	}
}

