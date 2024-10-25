//goal: convert a processing sketch into a series of belt lengths
import java.util.Arrays;

void setup(){
  size(800,800);
  background(255);
  strokeWeight(10);
  fill(0);
}
float a, b;
int count = 0;
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

void keyPressed(){
 if(key=='f'){
   background(255);
   points.clear();
 }
 if(key == 'p'){
   float[] lengths = coordsToLengths(points); 
   println("Lengths of Cables: " + Arrays.toString(lengths));
   print("Coords: ");
   for(int[] coords : points) 
     print(Arrays.toString(coords) + " ");
   println();
   //to do: send to serial
 }
}

float[] coordsToLengths(ArrayList<int[]> points){
  float[] lengths = new float[points.size()*2];
  int i = 0;
  for(int[] coords : points){
    int x = coords[0];
    int y = coords[1];
    //to do: calculate lengths 
    lengths[i] = x;
    lengths[i + 1] = y;
    i += 2;
  }
  return lengths;
}
