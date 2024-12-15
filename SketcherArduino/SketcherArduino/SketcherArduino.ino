#include <AccelStepper.h>
#include <Adafruit_MotorShield.h>
#include <MultiStepper.h>
#include <Servo.h>
//for the left stepper, a positive position means moving counter clockwise
//for the right stepper, a positive position menas moving clockwise
//Drawing information 

float homeLengths[] = {1819, 1857}; //home 1819mm left stepper length, 1857mm right stepper length
float lengths[] = {1217.212, 1539.3882, 1433.1455, 1321.8173, 1433.1455, 1321.8173, 1265.9403, 1579.8893, 1265.9403, 1579.8893, 1311.4945, 1547.5042, 1335.1255, 1531.4363, 1335.1255, 1531.4363, 1410.5762, 1461.6464, 1431.154, 1439.5155, 1450.3547, 1415.9272, 1460.9175, 1400.3303, 1462.0807, 1397.7328, 1472.4894, 1376.0538, 1473.3561, 1373.1019, 1473.8962, 1369.7931, 1473.8962, 1369.7931, 1263.6534, 1440.2129, 1263.6534, 1440.2129, 1124.4309, 1595.9862, 1110.3373, 1645.509, 1127.6259, 1665.251, 1132.0017, 1668.2173, 1210.6865, 1664.3092, 1220.4696, 1661.8379, 1366.2947, 1597.5543, 1380.5883, 1588.1124, 1520.2244, 1456.6143, 1536.1693, 1434.7281, 1617.8684, 1288.4274, 1618.2126, 1282.6367, 1606.9779, 1188.093, 1585.2234, 1158.499, 1580.5002, 1158.9521, 1530.2496, 1187.8052, 1434.2179, 1273.6797, 1421.3937, 1286.4607, 1339.7709, 1368.3307, 1339.7709, 1368.3307, 1283.3289, 1424.1888, 1283.3289, 1424.1888};

int n = (int)sizeof(lengths)/sizeof(lengths[0]);

//Hardware settings
const float speed = 100; // in step per second
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
  stepperLeft.setCurrentPosition(round(homeLengths[0]/mmPerStep));
  stepperRight.setCurrentPosition(round(homeLengths[1]/mmPerStep));
}

void run(float lengths[]){

  Serial.println("Sketch Drawing in Progress...");
  for (int i = 0; i < n; i+=2){
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

    Serial.println("Point " + String(i/2 + 1) + "/" + String(n/2) + " Completed");

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