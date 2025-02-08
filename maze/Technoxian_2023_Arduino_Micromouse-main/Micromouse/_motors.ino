#include <SparkFun_TB6612.h>

#define PWMA 5
#define AIN1 7
#define AIN2 6
#define BIN1 9
#define BIN2 10
#define PWMB 11
#define STBY 8

const int offsetA = 1;
const int offsetB = 1;

Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

long distanceTravelled;

int P, D, I, previousError, PIDvalue, error;
int lsp = 100;
int rsp = 100;
float Kp = 1;
float Kd = 10;
float Ki = 0;

int minValues[7], maxValues[7], threshold[7], sensorArray[7];
int wallValues[5];

double encStart1 = 0;
double encStart2 = 0;




void moveForward(long blocks, int speed) {
  float Kp = 0.5;
  float Kd = 1;
  float Ki = 0.4;

  long steps = blocks * 1500;
  float currentSpeed = 30;
  encStart1 = myEnc1.read();
  encStart2 = myEnc2.read();

  while (newPosition1 < encStart1 + steps) {
    encUpdate();
    readWall();

    if (abs(encStart1 + steps - newPosition1) < 500) {
      if (currentSpeed > 35) currentSpeed -= 0.2;
    } else if (currentSpeed < speed) {
      currentSpeed += 0.05;
    }

    long error = (newPosition1 - encStart1) - (newPosition2 - encStart2);
    P = error;
    I += error;
    D = error - previousError;

    PIDvalue = (Kp * P) + (Ki * I) + (Kd * D);
    previousError = error;

    long wallError = calculateWallError();

    lsp = currentSpeed - PIDvalue + wallError;
    rsp = currentSpeed + PIDvalue - wallError;

    lsp = constrain(lsp, 0, 200);
    rsp = constrain(rsp, 0, 200);

    motor1.drive(lsp);
    motor2.drive(rsp);
  }
  motor1.drive(0);
  motor2.drive(0);
}

void turn(int angle, int speed) {
  float currentSpeed = 30;
  float steps = abs(angle) * 4.5;
  long endPos;
  int error;
  encStart1 = myEnc1.read();
  encStart2 = myEnc2.read();

  if (angle > 0) endPos = encStart1 + steps;
  else endPos = encStart1 - steps;

  while (abs(newPosition1 - endPos) > 10) {
    encUpdate();

    if (abs(endPos - newPosition1) < 500) {
      if (currentSpeed > 35) currentSpeed -= 0.2;
    } else {
      currentSpeed += 0.05;
      if (currentSpeed > speed) currentSpeed = speed;
    }

    error = abs(newPosition1 - encStart1) - abs(encStart2 - newPosition2);
    P = error;
    I += error;
    D = error - previousError;

    PIDvalue = (Kp * P) + (Ki * I) + (Kd * D);
    previousError = error;

    lsp = currentSpeed - PIDvalue;
    rsp = currentSpeed + PIDvalue;

    lsp = constrain(lsp, 0, 200);
    rsp = constrain(rsp, 0, 200);

    if (angle > 0) {
      motor1.drive(lsp);
      motor2.drive(-rsp);
    } else {
      motor1.drive(-lsp);
      motor2.drive(rsp);
    }
  }
  motor1.drive(0);
  motor2.drive(0);
}

void alignFront() {
  float speedFactor = 0.4;
  int lrOffset = wallValues[1] - wallValues[3];
  int alignUpperSpeed = 60;
  
  for (int i = 0; i < 1500; i++) {
    readWall();
    int y = 500 - sensorValue[2];
    int x = sensorValue[1] - sensorValue[3] + lrOffset;
    x *= 0.1;
    int leftSpeed = speedFactor * (y - x);
    int rightSpeed = speedFactor * (y + x);

    leftSpeed = constrain(leftSpeed, -alignUpperSpeed, alignUpperSpeed);
    rightSpeed = constrain(rightSpeed, -alignUpperSpeed, alignUpperSpeed);

    motor1.drive(leftSpeed);
    motor2.drive(rightSpeed);
  }
  motor1.drive(0);
  motor2.drive(0);
}

long calculateWallError() {
  long wallError = 0;

  if (sensorValue[0] < 200 && sensorValue[4] > 200) {
    wallError = 0.5 * (sensorValue[4] - 500);
  } else if (sensorValue[4] < 200 && sensorValue[0] > 200) {
    wallError = 0.5 * (500 - sensorValue[0]);
  } else if (sensorValue[0] < 300 || sensorValue[4] < 300) {
    wallError = 0;
  } else {
    wallError = 0.3 * (sensorValue[0] - sensorValue[4] + (wallValues[4] - wallValues[0]));
  }

  if (sensorValue[0] < 1 || sensorValue[4] < 1) wallError = 0;

  return wallError;
}

void encUpdate() {
  newPosition1 = myEnc1.read();
  newPosition2 = myEnc2.read();
}

void resetEnc() {
  newPosition1 = 0;
  oldPosition1 = -999;
  newPosition2 = 0;
  oldPosition2 = -999;
}

void motorTest() {
  Serial.println("Starting motor test...");
  motor1.drive(100);  // Drive motor1 forward
  motor2.drive(100);  // Drive motor2 forward
  delay(2000);        // Run for 2 seconds

  motor1.drive(-100); // Drive motor1 backward
  motor2.drive(-100); // Drive motor2 backward
  delay(2000);        // Run for 2 seconds

  motor1.drive(0);    // Stop motor1
  motor2.drive(0);    // Stop motor2
  Serial.println("Motor test completed.");
}

void calibrate() {
  digitalWrite(sensor_On_Pin, HIGH);

  for (int i = 0; i < 5; i++) {
    int j = i;
    if (i > 2) j = i + 3;
    minValues[i] = analogRead(j);
    maxValues[i] = analogRead(j);
  }

  for (int i = 0; i < 10000; i++) {
    motor1.drive(50);
    motor2.drive(-50);

    for (int i = 0; i < 5; i++) {
      int j = i;
      if (i > 2) j = i + 3;

      if (analogRead(j) < minValues[i]) {
        minValues[i] = analogRead(j);
      }
      if (analogRead(j) > maxValues[i]) {
        maxValues[i] = analogRead(j);
      }
    }
  }
  digitalWrite(sensor_On_Pin, LOW);
  
  for (int i = 0; i < 5; i++) {
    threshold[i] = (minValues[i] + maxValues[i]) / 2;
    Serial.print(threshold[i]);
    Serial.print(" ");
  }
  Serial.println();

  motor1.drive(0);
  motor2.drive(0);
}

void readWall() {
  digitalWrite(sensor_On_Pin, HIGH);
  for (int i = 0; i < 5; i++) {
    int j = i;
    if (i > 2) j = i + 3;
    sensorValue[i] = map(analogRead(j), minValues[i], maxValues[i], 0, 1000);
    sensorValue[i] = constrain(sensorValue[i], 0, 1000);
  }
  digitalWrite(sensor_On_Pin, LOW);
}
