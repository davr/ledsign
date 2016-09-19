#include "main.h"

int mode = MODE_LIFE;

void init_pins() {
  
  for(int i=0;i<8;i++) {
    pinMode(dPins[i], OUTPUT);
    digitalWrite(dPins[i], LOW);
  }
  
  for(int i=0;i<3;i++) {
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
  if(mode == MODE_TETRIS)
    loop_tetris();
  else
    loop_life();
    
  if(millis() - last_time > 30000) {
    if(mode == MODE_TETRIS)
      mode = MODE_LIFE;
    else
      mode = MODE_TETRIS;
    last_time = millis();
    randomize_display();
  }
}

