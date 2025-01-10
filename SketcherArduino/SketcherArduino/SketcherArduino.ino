#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>
#include <MultiStepper.h>
#include <Servo.h>
//for the left stepper, a positive position means moving counter clockwise from the user's perspective
//for the right stepper, a positive position menas moving clockwise from the user's perspective

float homeLengths[] = {1819, 1857}; //home 1819mm left stepper length, 1857mm right stepper length
float lengths[] = {};

int n = (int)sizeof(lengths)/sizeof(lengths[0]);

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

//DEBUG: Check if this allows me to automatically start repeating the sequence on the next line.
void loop(){
  while (Serial.available() > 0) {
    float lengthLeft = Serial.parseFloat();
    float lengthRight = Serial.parseFloat();
    if (Serial.read() == '\n') {
      run(lengthLeft, lengthRight);
      Serial.println("Point Complete");
    }
  }
}

void run(float leftLength, float rightLength){    
  float leftLength = lengths[i];
  float rightLength = lengths[i+1];

  long positions[2]; // Array of desired stepper positions
  
  positions[0] = round(leftLength/mmPerStep); // position of the left belt in steps
  positions[1] = round(rightLength/mmPerStep); // position of the right belt in steps
  Serial.println("Left Side: " + String(lengths[i]) + " Right Side: " + String(lengths[i+1]));
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

void checkSerial(){
  if (Serial.available() > 0) {
    // read the incoming byte:
    inputString = Serial.readString();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(inputString);
  }
}

void loop(){
  checkSerial();

  if (inputString.equals("begin")){
    isStarted = true;
  }

  if (isStarted == true){
    isStarted = false;
    inputString = "";
    run(lengths);
  }

}