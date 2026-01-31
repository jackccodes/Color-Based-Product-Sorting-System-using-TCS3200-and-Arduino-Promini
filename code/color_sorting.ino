#include <Adafruit_GFX.h> 
#include <Adafruit_SSD1306.h> 
#include <Servo.h> 
 
Servo servo1;  // Positioning servo 
Servo servo2;  // Gate servo 
 
 
 
// OLED setup 
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); 
 
// TCS3200 pin definitions 
const int s0 = 9; 
const int s1 = 8; 
const int s2 = 7; 
const int s3 = 6; 
const int out = 5; 
 
int red, green, blue; 
String detectedColor = ""; 
 
void setup() { 
 
   serial.begin(9600); 
 
   servo1.attach(3); 
   servo2.attach(4); 
   servo2.write(0);  // Keep gate initially closed 
 
 
 
    // Set TCS3200 pins 
    pinMode(s0, OUTPUT); 
    pinMode(s1, OUTPUT); 
    pinMode(s2, OUTPUT); 
    pinMode(s3, OUTPUT); 
    pinMode(out, INPUT); 
  
 
 
    digitalWrite(s0, HIGH); 
    digitalWrite(s1, HIGH); 
 
  // OLED init 
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("OLED not found")); 
    while (true); 
  } 
 
  display.clearDisplay(); 
  display.setTextSize(1); 
  display.setTextColor(WHITE); 
  display.setCursor(0, 0); 
  display.println("Color Sensor Init"); 
  display.display(); 
  delay(1000); 
} 
 
void loop() { 
  readColor(); 
  detectColor(); 
 
  Serial.print("R: "); Serial.print(red); 
  Serial.print(" G: "); Serial.print(green); 
  Serial.print(" B: "); Serial.println(blue); 
 
  // Display on OLED 
  display.clearDisplay(); 
  display.setCursor(0, 0); 
  display.print("R: "); display.println(red); 
  display.print("G: "); display.println(green); 
  display.print("B: "); display.println(blue); 
  display.print("Color: "); display.println(detectedColor); 
  display.display(); 
 
  delay(1000); 
} 
 
void readColor() { 
  digitalWrite(s2, LOW); digitalWrite(s3, LOW); 
  delay(100); 
  red = pulseIn(out, LOW); 
 
  digitalWrite(s2, HIGH); digitalWrite(s3, HIGH); 
  delay(100); 
  green = pulseIn(out, LOW); 
 
  digitalWrite(s2, LOW); digitalWrite(s3, HIGH); 

 
  delay(100); 
  blue = pulseIn(out, LOW); 
} 
 
void detectColor() { 
  // Match RGB ranges to known colors 
  if (inRange(red, 3, 10) && inRange(green, 8, 13) && inRange(blue, 13, 18)) { 
    detectedColor = "YELLOW"; 
  } else if (inRange(red, 5, 10) && inRange(green, 18, 23) && inRange(blue, 18, 
23)) { 
    detectedColor = "ORANGE"; 
  } else if (inRange(red, 8, 13) && inRange(green, 18, 23) && inRange(blue, 10, 
15)) { 
    detectedColor = "ROSE"; 
  } else if (inRange(red, 65, 100) && inRange(green, 60, 75) && inRange(blue, 
50, 70)) { 
    detectedColor = "BLACK"; 
  } else if (inRange(red, 5, 10) && inRange(green, 5, 10) && inRange(blue, 5, 10)) 
{ 
    detectedColor = "WHITE"; 
  } else if (inRange(red, 0, 40) && inRange(green, 40, 100) && inRange(blue, 40, 
100)) { 
    detectedColor = "RED"; 
  } else if (inRange(red, 40, 100) && inRange(green, 40, 100) && inRange(blue, 
0, 40)) { 
    detectedColor = "BLUE"; 
  } else if (inRange(red, 40, 100) && inRange(green, 0, 40) && inRange(blue, 40, 
100)) { 
    detectedColor = "GREEN"; 
  } else if (inRange(red, 18, 23) && inRange(green, 30, 35) && inRange(blue, 19, 
23)) { 
    detectedColor = "MAGENTA"; 
  } else { 
    detectedColor = "UNKNOWN"; 
  } 
} 
 
bool inRange(int val, int minVal, int maxVal) { 
  return val >= minVal && val <= maxVal; 
} 
 
// Move servo1 to bin based on detectedColor 
if (detectedColor == "RED") { 
  servo1.write(0); 
} else if (detectedColor == "GREEN") { 
  servo1.write(30); 
} else if (detectedColor == "BLUE") { 
  servo1.write(60); 
} else if (detectedColor == "YELLOW") { 
servo1.write(90); 
} else if (detectedColor == "BLACK") { 
servo1.write(120); 
} else if (detectedColor == "WHITE") { 
servo1.write(150); 
} else { 
// Unknown color - no movement 
servo1.write(90);  // Neutral 
} 
delay(500);  // Allow time for servo1 to rotate 
// Open gate with servo2 
servo2.write(90);   // Open 
delay(1000);        
// Wait for ball to drop 
servo2.write(0);    // Close gate 
