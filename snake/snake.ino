#include <LedControl.h>

int x[64], y[64];
int dots = 2;

int DIN = 12;
int CS =  11;
int CLK = 10;

int xPosition, yPosition = 0;
int foodX, foodY;

int direction = 1;
int gameRunning = 1;

LedControl lc = LedControl(DIN,CLK,CS,1);

void setup() {

  Serial.begin(9600); 

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  
  lc.shutdown(0, false);       // The MAX72XX is in power-saving mode on startup
  lc.setIntensity(0, 15);      // Set the brightness to maximum value
  lc.clearDisplay(0);

  randomSeed(analogRead(A4));

  for (int i = 0; i < dots; i++) {
    x[i] = 1 + i;
    y[i] = 1;
  }

  spawnFood();
}

void loop() { 

  if (gameRunning == 1) {
    readJoystick();
    moveSnake();
    renderMap();
    readJoystick();
  }

  delay(500 - 10*dots);
}

void readJoystick() {
  
  xPosition = analogRead(A0);
  yPosition = analogRead(A1);

  if (xPosition > 750 and direction != 2) {
    direction = 1; // Right
   
  } else if (xPosition < 250 and direction != 1) {
    direction = 2; // Left
    
  } else if (yPosition < 250 and direction != 4) {
    direction = 3; // Up
    
  } else if (yPosition > 750 and direction != 3) {
    direction = 4; // Down
  }
}

void spawnFood() {

  foodX = random(0,7);
  foodY = random(0,7);

  for (int i = 0; i < dots; i++) {
    if (x[i] == foodX and y[i] == foodY) {
      spawnFood();
    }
  }
}

void moveSnake() {
  for (int i = dots; i > 0; i--) {
    x[i] = x[i - 1];
    y[i] = y[i - 1];
  } 

  if (direction == 1) {
    x[0] = x[0] + 1;
  } else if (direction == 2) {
    x[0] = x[0] - 1;
  } else if (direction == 3) {
    y[0] = y[0] - 1;
  } else if (direction == 4) {
    y[0] = y[0] + 1;
  }
}

void renderMap() {

  youLose();

  if (gameRunning == 1) {
    lc.clearDisplay(0);
    lc.setLed(0,foodY,foodX, true);

    for (int i = 0; i < dots; i++) {
    
      lc.setLed(0,y[i],x[i], true);
    
      if (x[i] == foodX and y[i] == foodY) {
        dots = dots + 1;
        spawnFood();
      }
    }
  }
}

void youLose() {
  for (int i = 0; i < dots; i++) {
    if (x[i] > 7 or y[i] > 7 or x[i] < 0 or y[i] < 0) {
      gameRunning = 0;
    }
    if (x[0] == x[i] and y[0] == y[i] and i != 0) {
      gameRunning = 0;
    }
  }

  if (gameRunning == 0) {
    
    for (int i = 0; i < (dots + 1); i++) {
      lc.setLed(0,y[i],x[i], false);
      delay(50);
    }
    
    delay(500); 
    
    for (int i = 0; i < (dots + 1); i++) {
      lc.setLed(0,y[i],x[i], true);
    }
  }
}
