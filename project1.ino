const byte debugPin = 13;

const int pin2 = 2;
const int pin3 = 3;
const int pin4 = 4;
const int pin5 = 5;
const int pin6 = 6;
const int pin7 = 7;
const int pin8 = 8;
const int pin9 = 9;
const int pin10 = 10;
const int pin11 = 11;
const int pin12 = 12;
const int pinA1 = A1;
const int pinA2 = A2;
const int pinA3 = A3;
const int pinA4 = A4;
const int pinA5 = A5;

const int buttons = A6;

int buttonValue = 0;
int state = 0;
bool changing = false;

long interval = 1000;
long previousMillis = 0;

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once: 
  pinMode(pin2, OUTPUT); 
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);
  pinMode(pin5, OUTPUT); 
  pinMode(pin6, OUTPUT);
  pinMode(pin7, OUTPUT);
  pinMode(pin8, OUTPUT);
  pinMode(pin9, OUTPUT);
  pinMode(pin10, OUTPUT);
  pinMode(pin11, OUTPUT);
  pinMode(pin12, OUTPUT);
  pinMode(pinA1, OUTPUT);
  pinMode(pinA2, OUTPUT);
  pinMode(pinA3, OUTPUT);
  pinMode(pinA4, OUTPUT);
  pinMode(pinA5, OUTPUT);

  pinMode(buttons, INPUT);
  
  pinMode(debugPin, OUTPUT);

  //reset row pin to off
  digitalWrite(pin5, HIGH); //A
  digitalWrite(pin4, HIGH); //B
  digitalWrite(pin3, HIGH); //C
  digitalWrite(pin2, HIGH); //D
  digitalWrite(pin12, HIGH); //E
  digitalWrite(pin11, HIGH); //F
  digitalWrite(pin10, HIGH); //G
  digitalWrite(pinA1, HIGH); //J
}

void readButton() {
  buttonValue = analogRead(buttons);

  // unpressed : 1023
  // pressed : 400 - 430

  if (buttonValue >= 650) {
    //state = 0;
    changing = false;
  }

  if (buttonValue < 649 && buttonValue >=0 && !changing) {
    changing = true;
    state = !state;
    Serial.print("Value =");
    Serial.println(buttonValue);
    Serial.print("State =");
    Serial.println(state);
    delay(10);
  }
  

/*
  if (buttonValue < 800 && buttonValue > 650) {
    state = 0;
  }
  if (buttonValue < 649 && buttonValue >=0) {
    state = 1;
  }
  */
}

void rowOn(int rowPin, int bits) {
  // turn on row pin 
  digitalWrite(rowPin, LOW);

  // turn on col pin based on bits pattern
  if ((bits>>7) & 0b1) digitalWrite(pinA5, HIGH); //1
  if ((bits>>6) & 0b1) digitalWrite(pinA4, HIGH); //2
  if ((bits>>5) & 0b1) digitalWrite(pinA3, HIGH); //3
  if ((bits>>4) & 0b1) digitalWrite(pinA2, HIGH); //4
  if ((bits>>3) & 0b1) digitalWrite(pin6, HIGH); //5
  if ((bits>>2) & 0b1) digitalWrite(pin7, HIGH); //6
  if ((bits>>1) & 0b1) digitalWrite(pin8, HIGH); //7
  if (bits & 0b1) digitalWrite(pin9, HIGH); //8


  // turn off all
  digitalWrite(rowPin, HIGH);
  // reset col pin to off
  digitalWrite(pinA5, LOW); //1
  digitalWrite(pinA4, LOW); //2
  digitalWrite(pinA3, LOW); //3
  digitalWrite(pinA2, LOW); //4
  digitalWrite(pin6, LOW); //5
  digitalWrite(pin7, LOW); //6
  digitalWrite(pin8, LOW); //7
  digitalWrite(pin9, LOW); //8
}

void O() {
  // O
  rowOn(pin5, 0x7E);
  rowOn(pin4, 0xE7);
  rowOn(pin3, 0xC3);
  rowOn(pin2, 0xC3);
  rowOn(pin12, 0xC3);
  rowOn(pin11, 0xC3);
  rowOn(pin10, 0xE7);
  rowOn(pinA1, 0x7E);
}
void K() {
  // K
  rowOn(pin5, 0x63);
  rowOn(pin4, 0x66);
  rowOn(pin3, 0x6C);
  rowOn(pin2, 0x78);
  rowOn(pin12, 0x6C);
  rowOn(pin11, 0x66);
  rowOn(pin10, 0x63);
  rowOn(pinA1, 0x61);
}
void smiley() {
  // :)
  rowOn(pin4, 0b00100100);
  rowOn(pin3, 0b00100100);
  rowOn(pin2, 0b00100100);
  rowOn(pin12,0b01000010);
  rowOn(pin11,0b00111100);
}

void loop() {
  // alternate every second
  /*
  unsigned long currentMillis = millis();
  
  if (currentMillis % 2000 < 1000) {
    O();
  } else {
    K();
  }
  */
  unsigned long currentMillis = millis();
  
  readButton();
/*
  if (buttonLState == HIGH) { // left button play OK
    if (currentMillis % 2000 < 1000) {
      O();
    } else {
      K();
    }
    digitalWrite(debugPin, LOW);
  } else {
    digitalWrite(debugPin, HIGH);
  }
*/
  
  if (state == 0) { // right button play :)
    smiley();
    readButton();
  }

  if (state == 1) {
    // alternate O K
    if (currentMillis % 2000 < 1000) {
      O();
    } else {
      K();
    }
    readButton();
  }
  
  
  

  
}
