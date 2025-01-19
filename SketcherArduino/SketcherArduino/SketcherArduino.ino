#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>
#include <MultiStepper.h>
#include <Servo.h>
//for the left stepper, a positive position means moving counter clockwise from the user's perspective
//for the right stepper, a positive position menas moving clockwise from the user's perspective

float homeLengths[] = {1819, 1857}; //home 1819mm left stepper length, 1857mm right stepper length

//Hardware settings
const float speed = 100; //max speed in step per second
const int dotDelay = 2000;
const float pulleyRadius = 6.5;//in mm. 40/pi/2
const float stepDeg = 0.031;//in rad
const float mmPerStep = pulleyRadius*stepDeg; //0.2015mm Think of this as the resolution of the plotter. 
const int servoPin = 10;
const int servoUp = 90; //deg for pen to be up
const int servoDown = 180; // deg for pen to be down

String inputString = "";     // A String to hold incoming data
bool isStarted = false;

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Servo penServo;

// Connect two steppers with 200 steps per revolution (1.8 degree)
Adafruit_StepperMotor *myStepper1 = AFMS.getStepper(200, 1); //left stepper
Adafruit_StepperMotor *myStepper2 = AFMS.getStepper(200, 2); //right stepper

MultiStepper steppers;

void forwardstep1() {
  myStepper1->onestep(FORWARD, DOUBLE);
}
void backwardstep1() {
  myStepper1->onestep(BACKWARD, DOUBLE);
}

void forwardstep2() {
  myStepper2->onestep(FORWARD, DOUBLE);
}
void backwardstep2() {
  myStepper2->onestep(BACKWARD, DOUBLE);
}

// Now we'll wrap the 2 steppers in an AccelStepper object
AccelStepper stepperLeft(forwardstep1, backwardstep1);
AccelStepper stepperRight(forwardstep2, backwardstep2);

void setup()
{
  AFMS.begin();
  penServo.attach(servoPin);

  Serial.begin(9600);

  // Configure each stepper
  stepperLeft.setMaxSpeed(speed);
  stepperRight.setMaxSpeed(speed);

  // Then give them to MultiStepper to manage
  steppers.addStepper(stepperLeft);
  steppers.addStepper(stepperRight);
  stepperLeft.setCurrentPosition(round(homeLengths[0]/mmPerStep));
  stepperRight.setCurrentPosition(round(homeLengths[1]/mmPerStep));
}

void loop(){
  if (Serial.available() > 0) {
    String msg = Serial.readString();
    
    if (msg[0] == 'M') {
      int lengths[2];
      parseLengths(msg, lengths, 2);
      run(lengths[0], lengths[1]);
      Serial.println("Point Complete");
    }
  }
}

void parseLengths(String input, int* outputArray, int arraySize) {
  int currentIndex = 0;  // Index for the output array

  // Start parsing from the second character to skip the initial letter
  int startIdx = 1;  // Skip the first character (the letter)

  // Use a while loop to extract integers
  while (currentIndex < arraySize) {
    // Find the next comma
    int endIdx = input.indexOf(',', startIdx);

    // If no more commas, extract the last number
    if (endIdx == -1) {
      endIdx = input.length();
    }

    // Extract the substring, convert to int, and store in the array
    String numberStr = input.substring(startIdx, endIdx);
    outputArray[currentIndex] = numberStr.toInt();

    // Move to the next segment
    startIdx = endIdx + 1;
    currentIndex++;
  }
}

void run(float leftLength, float rightLength){    

  long positions[2]; // Array of desired stepper positions
  
  positions[0] = round(leftLength/mmPerStep); // position of the left belt in steps
  positions[1] = round(rightLength/mmPerStep); // position of the right belt in steps
  steppers.moveTo(positions); //position is in steps and moves to the position not by a distance
  steppers.runSpeedToPosition(); // Blocks until all are in position

  penDown(true);
  delay(dotDelay);
  penDown(false);
}

void penDown(bool isDown){
  if(isDown){  
    penServo.write(servoDown);
  }
  else{  
    penServo.write(servoUp);
  }
}