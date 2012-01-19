
const int ledPin = 11;   // Teensy has LED on 11, Teensy++ on 6

// data pins on the shift registers
const int dPins[] = {0,1,2,3,13,14,15,4};

// row pins on the 3-to-8 decoders
const int rPins[] = {18,17,16};

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

void setup() {
  Serial.begin(9600);

  init_pins();
  init_display();
}

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

}

void loop() { 
  loop_tetris();
}

