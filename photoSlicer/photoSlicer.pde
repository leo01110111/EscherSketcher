PImage photo;

void setup() {
  size(400, 400);
  photo = loadImage("test.png");
  photo.loadPixels();
  int wid = photo.width;
  int h = photo.height;
  int dim = h*wid;
  //draws a black and white image by turning the dark pixels into dots and ignoring the light pixels
  float threshold = 200;
  float imageWid = 500; //in mm, length is determined by ratio of the image imported
  float imageHeight = imageWid*(float(h)/float(wid)); //in mm
  int imageX = 0; // where the top left corner of the image would be on the canvas
  int imageY = 0; 
  
  ArrayList<Point> points = new ArrayList<Point>();
  
  int row = 0;
  int col = 0;
  
 for (int d = 0; d < dim; d++){
   
   if(d%wid == 0 && d != 0){
     row++;
   }
   
   col = d - row*wid;
   
   if (brightness(photo.pixels[d]) <= threshold){
      photo.pixels[d] = color(255, 0, 0); 
      //println(row* (imageHeight/h));
        Point point = new Point(round(col * (imageWid/wid)),round(row * (imageHeight/h)));
        points.add(point);
   }
 }
   
 for (Point p : points) {
    print(p.x + ", " + p.y);
 }
 photo.updatePixels();
}

void draw() {image(photo, 0, 0);}

class Point{
  float x;
  float y;
  
  Point(float xi, float yi){
    x = xi;
    y = yi;
  }
}
