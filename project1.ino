// @kitetale
// Snake Game 8x8x2 game
// HCI 05-333 Gadgets project 1 code
// 10/11/2022

const byte debugPin = 13;
// pins 
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

// button related 
const int buttons = A6;
int buttonValue = 0;
int state = 4;
int lastState= 4;

// locks for button press state setting 
bool changing1 = false;
bool changing2 = false;
bool changing3 = false;
bool changing4 = false;

// initialize board 
int board[8] = { 0b00000000,
                 0b00000000,
                 0b00000000,
                 0b00011100,
                 0b00000000,
                 0b00000000,
                 0b01000000,
                 0b00000000  };
                 
// snake (player)                
int snake[16][2] = {{3,3}, {3,4}, {3,5}}; // tracking snake pixel locations
int snakeLength = 3; // snake's length
int maxLength = 16;

// food (target)
int foodLoc[2] = {6,1};

// row pins in order
int rowPins[8] = {pin5, pin4, pin3, pin2, pin12, pin11, pin10, pinA1};

// timer for each snake body to move by
unsigned long currentTime;
unsigned long lastTime = 0;
int tSpeed = 300; //millisecond to move pix by

// game over indicator
bool gameOver = false;

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

/* read buttons analog pin and set direction state based on button pressed */
void readButton() {
  if (gameOver) {
    state = 4;
    lastState = 4;
    return;
  }
  
  buttonValue = analogRead(buttons);
/*
  Serial.print("Value =");
  Serial.println(buttonValue);
*/

  if (buttonValue <=300 && !changing1) {
    state = 0;
    lastState = state;
    changing1 = true;
    changing2 = false;
    changing3 = false;
    changing4 = false;

    Serial.print("Value =");
    Serial.println(buttonValue);
    Serial.print("State =");
    Serial.println(state);
  }

  if (buttonValue > 300 && buttonValue <= 630  && !changing2) {
    state = 1;
    lastState = state;
    changing1 = false;
    changing2 = true;
    changing3 = false;
    changing4 = false;

    Serial.print("Value =");
    Serial.println(buttonValue);
    Serial.print("State =");
    Serial.println(state);
  }

  if (buttonValue < 900 && buttonValue > 630 && !changing3) {
    state = 2;
    lastState = state;
    changing1 = false;
    changing2 = false;
    changing3 = true;
    changing4 = false;

    Serial.print("Value =");
    Serial.println(buttonValue);
    Serial.print("State =");
    Serial.println(state);
  }

  if (buttonValue >= 900 && buttonValue < 1000 && !changing4) {
    state = 3;
    lastState = state;
    changing1 = false;
    changing2 = false;
    changing3 = false;
    changing4 = true;

    Serial.print("Value =");
    Serial.println(buttonValue);
    Serial.print("State =");
    Serial.println(state);
  }

  if (buttonValue > 1000) {
    changing1 = false;
    changing2 = false;
    changing3 = false;
    changing4 = false;
    
    if (lastState != 4) {
      if (currentTime - lastTime >= tSpeed) {
        state = lastState;
        moveSnake(); // updates the board based on state every second
        lastTime = currentTime;
      }
    }
    state = 4;
  }
}

/* set [rowPin] row's column in [bits] pattern */
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

  delay(1);

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

/* Food (target) function */
// generate new location for food
void generateFood() {
  foodLoc[0] = random(random(88) + random(28)) % 8;
  foodLoc[1] = random(random(78) + random(42)) % 8;
}

/* board functions */
// print board
void drawBoard() {
  for (int i=0; i<8; i++) {
    rowOn(rowPins[i],board[i]);
  }
}

// update board array
void updateBoard() {
  if (gameOver) {
    gg();
    return;
  }
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

  //update food location on the board
  board[foodLoc[0]] = board[foodLoc[0]] | (1<<(7-foodLoc[1]));

  // draw updated board
  drawBoard();
}

/* snake functions */
// move snake location array in the direction set by button
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

  // for when snake head ate food
  int lastPosR = snake[snakeLength-1][0];
  int lastPosC = snake[snakeLength-1][1];

  Serial.print("snake = [");
  Serial.print("[");
  Serial.print((snake[0][0] + rowMove + 8) % 8);
  Serial.print(",");
  Serial.print((snake[0][1] + colMove + 8) % 8);
  Serial.print("], ");
  
  //update snake location (move by one index)
  for (int i=snakeLength-1; i>0; i--) {
    snake[i][0] = snake[i-1][0];
    snake[i][1] = snake[i-1][1];

    Serial.print("[");
    Serial.print(snake[i][0]);
    Serial.print(",");
    Serial.print(snake[i][1]);
    Serial.print("], ");
  }
  snake[0][0] = (snake[0][0] + rowMove + 8) % 8;
  snake[0][1] = (snake[0][1] + colMove + 8) % 8;
  Serial.println("]");

  // check if has got to food location
  if (snake[0][0] == foodLoc[0] && snake[0][1] == foodLoc[1]) {
    generateFood(); //set new food location
    if (snakeLength < maxLength){ // bounding for memory safety
      // add last index to snake array 
      snake[snakeLength][0] = lastPosR;
      snake[snakeLength][1] = lastPosC;
      snakeLength ++;

      if (snakeLength == 8) {
        tSpeed = 500;
      }
      if (snakeLength == 12) {
        tSpeed = 800;
      }
    }
  }

  updateBoard();

  if (checkCollision()) {
     gameOver = true;
     gg();
  }
}

bool checkCollision() {
  for (int i = 1; i < snakeLength; i++) {
      if (snake[i][0] == snake[0][0] &&
          snake[i][1] == snake[0][1] ) {
        // snake collides!
        return true;
      }
  }
  return false;
}


void loop() {
  currentTime = millis();
  // left : 0 , up : 1, right : 2, down: 3
  readButton(); // read button input
  drawBoard(); // draws on 8x8 led board
}

/* game end screen */
void gg() {
  board[0] = 0b11110010;
  board[1] = 0b10000111;
  board[2] = 0b10110000;
  board[3] = 0b10011111;
  board[4] = 0b11111000;
  board[5] = 0b00001011;
  board[6] = 0b00001001;
  board[7] = 0b00001111;

  drawBoard();
}
