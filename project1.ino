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
int state = 4;
bool changing1 = false;
bool changing2 = false;
bool changing3 = false;
bool changing4 = false;

// initialize board 
int board[8] = { 0b00000000,
                 0b00000000,
                 0b00000000,
                 0b00010000,
                 0b00000000,
                 0b00000000,
                 0b00000000,
                 0b00000000  };
                 
// snake (player)                
int snake[8][2] = {{3,3}}; // tracking snake pixel locations
int snakeLength = 1; // snake's length

// row pins in order
int rowPins[8] = {pin5, pin4, pin3, pin2, pin12, pin11, pin10, pinA1};

unsigned long currentTime;

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
/*
  Serial.print("Value =");
  Serial.println(buttonValue);
*/

  if (buttonValue <=300 && !changing1) {
    state = 0;
    changing1 = true;
    changing2 = false;
    changing3 = false;
    changing4 = false;

    Serial.print("Value =");
    Serial.println(buttonValue);
    Serial.print("State =");
    Serial.println(state);
    delay(10);
  }

  if (buttonValue > 300 && buttonValue <= 630  && !changing2) {
    state = 1;
    changing1 = false;
    changing2 = true;
    changing3 = false;
    changing4 = false;

    Serial.print("Value =");
    Serial.println(buttonValue);
    Serial.print("State =");
    Serial.println(state);
    delay(10);
  }

  if (buttonValue < 900 && buttonValue > 630 && !changing3) {
    state = 2;
    changing1 = false;
    changing2 = false;
    changing3 = true;
    changing4 = false;

    Serial.print("Value =");
    Serial.println(buttonValue);
    Serial.print("State =");
    Serial.println(state);
    delay(10);
  }

  if (buttonValue >= 900 && buttonValue < 1000 && !changing4) {
    state = 3;
    changing1 = false;
    changing2 = false;
    changing3 = false;
    changing4 = true;

    Serial.print("Value =");
    Serial.println(buttonValue);
    Serial.print("State =");
    Serial.println(state);
    delay(10);
  }

  if (buttonValue > 1000) {
    changing1 = false;
    changing2 = false;
    changing3 = false;
    changing4 = false;
    
    if (state != 4) moveSnake(); // updates the board based on state every second
    state = 4;
  }



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

  delay(2);

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

/* board functions */
void drawBoard() {
  for (int i=0; i<8; i++) {
    rowOn(rowPins[i],board[i]);
  }
}

void updateBoard() {
  // reset board
  for (int i=0; i<8; i++) {
    board[i] = 0b00000000;
  }
  
  //update board with snake location
  int row = 0;
  int col = 0;
  int newRow = 0;
  for (int i=0; i<snakeLength; i++) {
    row = snake[i][0];
    col = snake[i][1];
    board[row] = board[row] | (1<<(7-col));
  }
  drawBoard();
}

void moveSnake() {
  int rowMove = 0;
  int colMove = 0;
  
  switch (state) {
    case 0 : // left
      colMove = -1;
      break;
     case 1 : // up
      rowMove = -1;
      break;
     case 2 : // right
      colMove = 1;
      break;
     case 3 : // down
      rowMove = 1;
      break;
     default : // state == 4
      rowMove = 0;
      colMove = 0;
      break;
  }

  Serial.print("snake = [");
  for (int i=0; i<snakeLength; i++) {
    snake[i][0] = (snake[i][0] + rowMove + 8) % 8;
    snake[i][1] = (snake[i][1] + colMove + 8) % 8;

    Serial.print("[");
    Serial.print(snake[i][0]);
    Serial.print(",");
    Serial.print(snake[i][1]);
    Serial.print("], ");
  }
  Serial.println("]");

  updateBoard();
}

void loop() {
  currentTime = millis();
  
  // left : 0 , up : 1, right : 2, down: 3
  readButton();
  drawBoard(); // draws on 8x8 led board
  

  
  if (state == 0) {
    //left();
    readButton();
  }

  if (state == 1) {
    //up();
    readButton();
  }

  if (state == 2) {
    //right();
    readButton();
  }

  if (state == 3) {
    //down();
    readButton();
  }
}




/* Example drawings */
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

void right() {
  rowOn(pin5,  0b00001000);
  rowOn(pin4,  0b00001100);
  rowOn(pin3,  0b00001110);
  rowOn(pin2,  0b11111111);
  rowOn(pin12, 0b11111111);
  rowOn(pin11, 0b00001110);
  rowOn(pin10, 0b00001100);
  rowOn(pinA1, 0b00001000);
}
void left() {
  rowOn(pin5,  0b00010000);
  rowOn(pin4,  0b00110000);
  rowOn(pin3,  0b01110000);
  rowOn(pin2,  0b11111111);
  rowOn(pin12, 0b11111111);
  rowOn(pin11, 0b01110000);
  rowOn(pin10, 0b00110000);
  rowOn(pinA1, 0b00010000);
}
void up() {
  rowOn(pin5,  0b00011000);
  rowOn(pin4,  0b00111100);
  rowOn(pin3,  0b01111110);
  rowOn(pin2,  0b11111111);
  rowOn(pin12, 0b00011000);
  rowOn(pin11, 0b00011000);
  rowOn(pin10, 0b00011000);
  rowOn(pinA1, 0b00011000);
}
void down() {
  rowOn(pin5,  0b00011000);
  rowOn(pin4,  0b00011000);
  rowOn(pin3,  0b00011000);
  rowOn(pin2,  0b00011000);
  rowOn(pin12, 0b11111111);
  rowOn(pin11, 0b01111110);
  rowOn(pin10, 0b00111100);
  rowOn(pinA1, 0b00011000);
}
