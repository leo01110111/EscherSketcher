#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>
#include <MultiStepper.h>
#include <Servo.h>
//for the left stepper, a positive position means moving counter clockwise
//for the right stepper, a positive position menas moving clockwise
//Drawing information 

float homeLengths[] = {1819, 1857}; //home 181.9cm left stepper length, 185.7cm right stepper length
float lengths[] = {1819,1857};

//Hardware settings
const float speed = 100; // in step per second
const int dotDelay = 2000;
const float pulleyRadius = 6.5;//in mm. 40/pi/2
const float stepDeg = 0.031;//in rad
const float mmPerStep = pulleyRadius*stepDeg; //in mm
const int servoPin = 10;
const int servoUp = 90; //deg for pen to be up
const int servoDown = 180; // deg for pen to be down

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
  initialize();
  //running the drawing
  run(lengths);
}

void initialize(){
  AFMS.begin();
  penServo.attach(servoPin);

  Serial.begin(9600);

  // Configure each stepper
  stepperLeft.setMaxSpeed(speed);
  stepperRight.setMaxSpeed(speed);

  // Then give them to MultiStepper to manage
  steppers.addStepper(stepperLeft);
  steppers.addStepper(stepperRight);
  stepperLeft.setCurrentPosition(homeLengths[0]);
  stepperRight.setCurrentPosition(homeLengths[1]);

}

void run(float lengths[]){
  Serial.println("Sketch Drawing in Progress...");
  for (int i = 0; i < sizeof(lengths); i+=2){
    float leftLength = lengths[i];
    float rightLength = lengths[i+1];

    long positions[2]; // Array of desired stepper positions
  
    positions[0] = leftLength/mmPerStep; // position of the left belt in steps
    positions[1] = rightLength/mmPerStep; // position of the right belt in steps
    Serial.println(positions[0]);
    steppers.moveTo(positions); //position is in steps and moves to the position not by a distance
    steppers.runSpeedToPosition(); // Blocks until all are in position

    penDown(true);
    delay(dotDelay);
    penDown(false);

    Serial.println("Point " + String(i/2 + 1) + "/" + String(sizeof(lengths)/2) + " Completed");

  }
}

void penDown(bool isDown){
  if(isDown){  
    penServo.write(servoDown);
  }
  else{  
    penServo.write(servoUp);
  }
}

void loop(){}