//goal: convert a processing sketch into a series of belt lengths
import java.util.Arrays;

void deadzones(){
  rect(0,0,width,110);
  rect(0,0,100,height);
  rect(width,0,-100,height);
  fill(255);
}

void setup(){
  size(2667,1270); //dimensions of reachable area in mm, drawing area is (2467x1160) (my macbook screen 2560 x 1600 pixels)
  background(255);
  strokeWeight(10);
  fill(0);
  deadzones();
}

float a, b;
int count = 0;
int offsetY = 0;
ArrayList<int[]> points = new ArrayList<int[]>();
long debounce = millis();
int pressDelay = 100;

void draw(){
 if(mousePressed && millis()-debounce > pressDelay){
   debounce = millis();
   int[] coords = {mouseX, mouseY};
   points.add(coords);
   ellipse(coords[0],coords[1], 1, 1);
 }
 
}
void mouseWheel(MouseEvent event) {
  // Adjust offset based on scroll direction
  float e = event.getCount(); // Scroll amount
  offsetY += e * 40;

  // Constrain offset to content bounds
  offsetY = constrain(offsetY, 0, height);
}
void keyPressed(){
 if(key=='f'){
   background(255);
   deadzones();
   points.clear();
 }
 if(key == 'p'){
   float[] lengths = coordsToLengths(points); 
   println("Lengths of Cables: " + Arrays.toString(lengths));
   //print("Coords: ");
   //for(int[] coords : points) 
   //  print(Arrays.toString(coords) + " ");
   //println();
 }
}

float[] coordsToLengths(ArrayList<int[]> points){
  float[] lengths = new float[points.size()*2];
  int i = 0;
  for(int[] coords : points){
    int x = coords[0];
    int y = coords[1];
    float leftLength = sqrt(pow(x,2)+pow(y,2)); //from an observer looking at the machine
    float rightLength = sqrt(pow(width-x,2)+pow(y,2));
    lengths[i] = leftLength;
    lengths[i + 1] = rightLength;
    i += 2;
  }
  return lengths;
}
