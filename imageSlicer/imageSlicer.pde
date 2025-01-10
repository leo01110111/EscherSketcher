// Processing code to convert a digital image into a 1D float array for motor commands
import java.util.Arrays;

PImage img;
float[] motorCommands;
float beltSpacing = 2667; // Distance between the two belts (real value)
int drawingSurfaceWidth = 2667; // Width of the drawing surface
int drawingSurfaceHeight = 1270; // Height of the drawing surface
float offsetX = 1333; // X offset to position the image's top-left corner
float offsetY = 645; // Y offset to position the image's top-left corner
float scaleFactor = 1;  // Scale factor to match image size to machine size
int resolution = 1; //res = 2 uses every other point 1 is normal

void setup() {
  // Set an initial size (fallback value) first
  size(2667, 1270);

  // Load your image after the size is set
  img = loadImage("circle.png"); // Load your image file
  println("Original Image Width: " + img.width);
  println("Original Image Height: " + img.height);
  
  // Position the image at the specified offsets
  image(img, offsetX, offsetY, img.width, img.height);

  
  motorCommands = generateMotorCommands(img);
  println("Motor commands:", Arrays.toString(motorCommands));
}

float[] generateMotorCommands(PImage img) {
  img.loadPixels();
  ArrayList<Float> commands = new ArrayList<Float>();

  for (int y = 0; y < img.height; y++) {
    for (int x = 0; x < img.width; x++) {
      int pixelIndex = x + y * img.width;
      color pixelColor = img.pixels[pixelIndex];

      // Use brightness to determine if this pixel should be included in the path
      if (brightness(pixelColor) < 128) { // Threshold for black/dark pixels
        // Adjust coordinates to account for offset and scaling
        float adjustedX = x * scaleFactor + offsetX;
        float adjustedY = y * scaleFactor + offsetY;

        float[] beltLengths = calculateBeltLengths(adjustedX, adjustedY);
        commands.add(beltLengths[0]);
        commands.add(beltLengths[1]);
      }
    }
  }

  // Convert ArrayList to float array
  int pointsScaled = roundDownToEven(commands.size()/(2*resolution));
  float[] result = new float[pointsScaled];
  int j = 0;
  for (int i = 0; i < pointsScaled-3; i+=2) {
      result[i] = commands.get(j);
      result[i+1] = commands.get(j+1);
      j += 2*resolution;
  }
  return result;
}

int roundDownToEven(int num) {
  return (num % 2 == 0) ? num : num - 1;
}

float[] calculateBeltLengths(float x, float y) {
  // Calculate belt lengths based on Cartesian coordinates
  float rightBeltLength = dist(x, y, beltSpacing, 0); // Right anchor point
  float leftBeltLength = dist(x, y, 0, 0);           // Left anchor point
  return new float[] { leftBeltLength, rightBeltLength };
}

void draw() {
  // Optionally visualize the path here
}
