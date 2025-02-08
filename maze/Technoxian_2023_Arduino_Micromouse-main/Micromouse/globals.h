#ifndef globals_h
#define globals_h

#include <stdint.h>
#include <Encoder.h>

#define north 0
#define east 1
#define south 2
#define west 3

#define leftSensor A0
#define diagonalLeftSensor A1
#define centreSensor A2
#define diagonalRightSensor A3
#define rightSensor A5

#define wallThreshold 120

#define sensor_On_Pin 17

#define button 3

#define absolute(number) (((number) > 0)? (number) : (-(number)))
#define minimum(num1, num2) (((num1) < (num2))? (num1) : (num2))

struct cell {
  uint8_t flood;
  uint8_t neighbours;
  uint8_t visited;
};

extern struct cell floodArray[];

extern const uint8_t rows, cols;

extern uint8_t startCell, startDir, targetCells[], currentCell;

extern uint8_t *values[];

extern int sensorValue[];

extern long newPosition1, newPosition2, oldPosition1, oldPosition2;

extern Encoder myEnc1;
extern Encoder myEnc2;

#endif
