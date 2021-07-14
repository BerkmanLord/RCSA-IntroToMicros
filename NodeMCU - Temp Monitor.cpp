/*
 * Thermometer & Humidity Sensor - RCS Academy - Lord
 * Measuring the temperature & humidity and displaying it on our OLED screen.
 * Also having fun with an animation!
 */

// Our animation variables.
int loadingNumber = 0;
char loadingIcon [2] = { '-', '|'}; 

// Display Libraries
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Setting-up Our Display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Setting-up Our Temp - Humidity Sensor
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN D3     // Digital pin connected to the DHT sensor 
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
#define DHTTYPE    DHT11     // DHT 11
//#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE); // Starting our sensor.

uint32_t delayMS; // Setting up a timer for saving some power and matching sensor read frequency.

void setup() {
  Serial.begin(115200); // Starting our serial console.

  // Initialize the display.
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed if OLED is not Detected, loop forever
  }


  // Initialize device.
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));
  // Print humidity sensor details.
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print  (F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print  (F("Driver Ver:  ")); Serial.println(sensor.version);
  Serial.print  (F("Unique ID:   ")); Serial.println(sensor.sensor_id);
  Serial.print  (F("Max Value:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Min Value:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Resolution:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;
}

void loop() {
  
  //Reset the display.
  display.clearDisplay();
  display.setTextColor(WHITE);

  // Delay between measurements.
  delay(delayMS);

  // Get temperature event and print its value.
  sensors_event_t event;

  dht.temperature().getEvent(&event);

  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));

    //Displaying temperature error.
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print("Temp. Error!");
  }
  else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));

    //Displaying temperature.
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print("Temp. :");
    display.print(event.temperature);
    display.print("C");
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));

    //Displaying humidity error.
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print("Hum. Error!");
  }
  else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));

    //Displaying humidity.
    display.setTextSize(1);
    display.setCursor(0,11);
    display.print("Hum.  :");
    display.print(event.relative_humidity);
    display.print("%");

  }
    

    //Displaying additional lines and the animation.
    display.setCursor(0,24);
    display.print(loadingIcon[loadingNumber]); // We are making an animation out of two icons in an array!
    display.setCursor(0,35);
    display.print("Additional Line 1");
    display.setCursor(0,46);
    display.print("Additional Line 2");
    display.setCursor(0,57);
    display.print("Additional Line 3");
    display.display();

    if (loadingNumber < 1) // This loop alternates between the characters in our loadingIcon array!
    {
      loadingNumber++;
    } else {
      loadingNumber = 0;
    }
    


}
