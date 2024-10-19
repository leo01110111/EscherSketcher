//goal: operate exactly how the arduino code would operate and show a visual of what the robot would do
//input: motor velocity and turning ticks
//output: visual data of the pen moving and leaving a path behind it
//my computer screen is 2560(w)*1600(h)px
//every px is 0.5mm 
int screenWidth = 800; //400 mm
int screenHeight = 800; //400 mm
float motorLeftX = 0; //motors are assumed to be at height 0 mm
float motorRightX = screenWidth;
float penX, penY; //coords of the pen

//functions:
//pos finder: from the given belt lengths, convert them into penX and penY coords
//interface: draws the pen pos as a circle leaving a trail and lines connecting it to the motors (corners)
//runner: feed the belt lengths accounting for drawing patterns over time (this function should be in a different program)

float[] LengthsToCoords(float leftBelt, float rightBelt){
  //think of the belts and the top width as sides of a triangle
  //use the law of cosines to find theta
  float theta = acos((pow(leftBelt,2)+pow(screenWidth,2)-pow(rightBelt,2))/(2*leftBelt*screenWidth)); //law of cosines
  float x = cos(theta)*leftBelt; //since cos(theta) = x/leftBelt
  float y = sin(theta)*leftBelt;
  float[] coords = {x,y};
  return coords;
}

void settings(){
  size(screenWidth, screenHeight);
}

void setup(){
  background(255);
  strokeWeight(2);
  textSize(14);
  fill(0);
}

void draw(){
  float top = 800;
  float leftBelt = sqrt(17)*160;
  float rightBelt = 4*sqrt(2)*160;
  float[] penCoords = LengthsToCoords(leftBelt, rightBelt);
  ellipse(penCoords[0],penCoords[1],10,10);
  text("Coordinates of the pen: " + str(penCoords[0])+", "+str(penCoords[1]), width/2, height/2);
}
