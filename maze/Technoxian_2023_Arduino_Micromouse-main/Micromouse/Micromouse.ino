#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#include "globals.h"
#include "menu.h"
#include "floodfill.h"
#include "save_maze.h"

#define AIN1 7
#define BIN1 9
#define AIN2 6
#define BIN2 10
#define PWMA 5
#define PWMB 11
#define STBY 8

Encoder myEnc1(2, 8);
Encoder myEnc2(3, 12);
SSD1306AsciiAvrI2c oled;

const uint8_t rows = 16, cols = 16;

struct cell floodArray[rows * cols];  // This array stores the flood value and neighbour data for all the cells

uint8_t targetCells[4], startCell, startDir;

int sensorValue[7];

long newPosition1, newPosition2, oldPosition1, oldPosition2;

void setup() {
  Serial.begin(9600); // Initialize serial communication for debugging

  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);
  
  pinMode(PWMA, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(leftSensor, INPUT);
  pinMode(rightSensor, INPUT);
  pinMode(centreSensor, INPUT);
  pinMode(diagonalLeftSensor, INPUT);
  pinMode(diagonalRightSensor, INPUT);
  
  pinMode(STBY, OUTPUT);
 digitalWrite(STBY, HIGH);

  pinMode(sensor_On_Pin, OUTPUT);

  oledSetup();
  updateMazeValuesFromEEPROM();

  Serial.println("Calibration starting...");

  calibrate();  // Calibrate first

  Serial.println("Calibration completed");
  Serial.println("Starting the loop...");
  
}

void loop() {
  initialise();

  Serial.println("Bot initialized and starting navigation...");

  while (currentCell != targetCells[0] && currentCell != targetCells[1] && currentCell != targetCells[2] && currentCell != targetCells[3]) {
    updateWalls();
    flood();
    updateTargetCell();
    goToTargetCell();
    floodArray[currentCell].visited = 1;

    Serial.print("Current Cell: ");
    Serial.println(currentCell);
  }

  Serial.println("Target reached, saving maze...");
  updateMazeValuesInEEPROM();
  Serial.println("Maze saved.");
   

}
