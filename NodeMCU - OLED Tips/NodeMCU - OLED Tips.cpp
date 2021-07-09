/*
 * Adding Text & Icons to Oled - RCS Academy - Lord
 * Just experiment with this code to add text to your screen!
 * Included is the icons.h header file!
 */

// Display Libraries
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <icons.h>

// Setting-up Our Display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



String stringText = "This is a String!";
int integerNumber = 12345678;

void setup(){
  Serial.begin(115200); // Setting up the serial console.

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed if OLED is not Detected, loop forever
  }

}

void loop(){

for (int carLocation = 0; carLocation < 128; carLocation++) {

          display.clearDisplay();
          display.setTextColor(WHITE);
            display.setCursor(10,0);
            display.setTextSize(2);
            display.print(F("Welcome!"));
            display.setTextSize(1);
            display.setCursor(5,20);
            display.print(stringText);
            display.setCursor(5,31);
            display.print(integerNumber);
            display.drawBitmap(carLocation, 35, custom_icon, 32, 32, 1);
          display.display();
}



}
