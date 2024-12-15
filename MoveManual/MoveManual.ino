//MoveManual moves the sketcher x,y in respect to the pen coordinate frame. The coordinates are entered in the Serial Monitor like this: "100,100"

#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>
#include <MultiStepper.h>
#include <Servo.h>
//for the left stepper, a positive position means moving counter clockwise
//for the right stepper, a positive position menas moving clockwise
//Drawing information 

//Hardware settings
const float speed = 600; // in step per second
const int dotDelay = 2000;
const float pulleyRadius = 6.5;//in mm. 40/pi/2
const float stepDeg = 0.031;//in rad
const float mmPerStep = pulleyRadius*stepDeg; //in mm
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
  stepperLeft.setCurrentPosition(0);
  stepperRight.setCurrentPosition(0);
}

void run(float lengths[]){

  Serial.println("Sketch Drawing in Progress...");
  for (int i = 0; i < sizeof(lengths); i+=2){
    float leftLength = lengths[i];
    float rightLength = lengths[i+1];

    long positions[2]; // Array of desired stepper positions
  
    positions[0] = round(leftLength/mmPerStep); // position of the left belt in steps
    positions[1] = round(rightLength/mmPerStep); // position of the right belt in steps
    Serial.println("Left Side: " + String(positions[0]) + " Right Side: " + String(positions[1]));
    steppers.moveTo(positions); //position is in steps and moves to the position not by a distance
    steppers.runSpeedToPosition(); // Blocks until all are in position
    stepperLeft.setCurrentPosition(0);
    stepperRight.setCurrentPosition(0);
    Serial.println("Point " + String(i/2 + 1) + "/" + String(sizeof(lengths)/2) + " Completed");

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

bool parseFloats(String input, float &float1, float &float2) {
  int commaIndex = input.indexOf(','); // Find the comma

  if (commaIndex == -1) {
    return false; // No comma found
  }

  // Split the string into two parts
  String part1 = input.substring(0, commaIndex);
  String part2 = input.substring(commaIndex + 1);

  // Convert the parts to floats
  float1 = part1.toFloat();
  float2 = part2.toFloat();

  // Check if the conversion was successful
  return (float1 != 0.0 || part1 == "0") && (float2 != 0.0 || part2 == "0");
}

void loop(){

  checkSerial();
  float x,y;
  if(parseFloats(inputString, x, y)){
    isStarted = true;
  }

  if (isStarted == true){
    isStarted = false;
    inputString = "";
    float command[] = {x,y};
    run(command);
  }

}