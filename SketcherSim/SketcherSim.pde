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
float penSize = 1; //diameter of the pen

//functions:
//pos finder: from the given belt lengths, convert them into penX and penY coords - check
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

float[] pathList = {336.88278, 642.40955, 336.88278, 642.40955, 336.88278, 642.40955, 338.15085, 640.5825, 344.49384, 630.13965, 353.44165, 623.6353, 355.6009, 623.58, 354.0353, 631.61774, 353.9096, 637.8495, 352.7974, 640.9883, 350.9259, 643.69946, 348.90112, 645.08295, 345.92773, 644.7216, 342.95773, 644.3757, 466.06116, 495.99698, 466.06116, 495.99698, 466.06116, 495.99698, 466.06116, 495.99698, 464.86557, 491.62994, 465.39554, 485.5852, 469.57214, 481.3502, 478.75253, 475.39877, 484.80923, 478.16315, 486.47406, 479.851, 482.12137, 488.71362, 476.73157, 496.46048, 472.67114, 500.6176, 469.29416, 500.6366, 465.9206, 500.68155, 467.7863, 497.61832, 469.42093, 495.9395, 471.87817, 493.42578, 448.01785, 747.7433, 448.01785, 747.7433, 455.38556, 752.18085, 476.69907, 760.02765, 492.40024, 757.40216, 510.90607, 745.26843, 528.62177, 731.73834, 545.1835, 717.51306, 555.49347, 704.1115, 566.48035, 688.8396, 581.4912, 666.13214, 586.4, 648.5869, 588.33496, 636.6616, 586.9012, 630.2801, 583.4672, 616.79333, 581.5677, 600.5173, 580.11896, 584.2414, 267.32004, 643.3195, 267.32004, 643.3195, 267.32004, 643.3195, 265.75363, 656.2202, 272.60596, 671.0544, 302.89932, 698.9621, 345.2202, 732.6507, 391.73715, 757.7981, 474.73465, 798.8573, 549.50616, 829.19055, 579.3306, 827.2992, 603.3548, 818.31354, 636.4629, 807.1462, 668.5881, 782.18286, 703.1543, 741.907, 721.5518, 697.8804, 719.3469, 641.7632, 701.56537, 594.133, 675.909, 547.77094, 632.21674, 484.66278, 592.11487, 432.89722, 548.4168, 405.90762, 498.13754, 402.17035, 454.98682, 424.04364, 389.17862, 477.76562, 338.30902, 528.444, 305.65503, 569.0562, 284.8719, 602.28894, 273.7243, 630.9715, 270.29614, 647.0394}; //a triangle testing path

void settings(){
  size(screenWidth, screenHeight);
}

void setup(){
  background(255);
  strokeWeight(2);
  textSize(14);
  fill(0);
  for (int i = 0; i < pathList.length; i+=2){
    float leftBelt = pathList[i];
    float rightBelt = pathList[i+1];
    float[] penCoords = LengthsToCoords(leftBelt, rightBelt);
    ellipse(penCoords[0],penCoords[1],penSize,penSize);
    println("Coordinates of the pen: " + str(penCoords[0])+", "+str(penCoords[1]));
  }
}

void draw(){
}
