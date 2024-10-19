
//goal: convert a processing sketch into a series of motor commands
void setup(){
  size(800,800);
  background(255);
  strokeWeight(10);
  fill(0);
}
float a, b;
int count = 0;
ArrayList<ArrayList<Integer>> paths = new ArrayList<ArrayList<Integer>>();
ArrayList<Integer> points = new ArrayList<Integer>();
void draw(){
 if(mousePressed){
   points.add(mouseX);
   points.add(mouseY);
   point(mouseX, mouseY);
    text(points.get(0+count),mouseX, mouseY);
       count++;
 }
 paths.add(count, points);
 points.clear();
 count++;
}
void keyPressed(){
 if(key=='f'){
   background(255);
   fill(0);
   paths.clear();
   points.clear();
 }
 if(key=='c'){
   trace();}
}

void trace(){
    //input: x and y of where pen should be
  //output: length of each line
  background(255);
  for (ArrayList<Integer> i : paths){
    for(int j = 0; j< i.size(); j+=2){
          text(i.get(j),width-100, 10);
      //background(255);
       //line(0,0,i.get(j), i.get(j+1));
       //line(width,0,i.get(j),i.get(j+1));
       //delay(1000);
    }
  }
    //a = sqrt(mouseX^2+mouseY^2);
    //b = sqrt((width-mouseX)^2+mouseY^2);
    //text(a,10, 10);
    //text(b,50, 10);
}
