/*
 * Web Based Messaging - RCS Academy - Lord
 * Software for our Web Controlled Pager.
 * You can send messages to our NodeMCU through web-based interface.
 * Place it to another room and message your friends & family!
 * Make a reminder screen for the fridge!
 */


// Wifi & Web Server Libraries
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Display Libraries
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define firstLed D4
#define secondLed D0

// Setting-up Our Display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//Our warning icon in duotone bitmap.
 unsigned char warning_icon[] =
{
	0b00000000, 0b10000000, //         #       
	0b00000001, 0b11000000, //        ###      
	0b00000001, 0b11000000, //        ###      
	0b00000011, 0b11100000, //       #####     
	0b00000011, 0b01100000, //       ## ##     
	0b00000111, 0b01110000, //      ### ###    
	0b00000110, 0b00110000, //      ##   ##    
	0b00001110, 0b10111000, //     ### # ###   
	0b00001100, 0b10011000, //     ##  #  ##   
	0b00011100, 0b10011100, //    ###  #  ###  
	0b00011000, 0b10001100, //    ##   #   ##  
	0b00111000, 0b00001110, //   ###       ### 
	0b00110000, 0b10000110, //   ##    #    ## 
	0b01111111, 0b11111111, //  ###############
	0b01111111, 0b11111111, //  ###############
	0b00000000, 0b00000000, //                 
};

//Our heart icon in duotone bitmap.
 unsigned char heart_icon[] =
{                
	0b00000000, 0b00000000, //                 
	0b00000000, 0b00000000, //                 
	0b00111100, 0b01111000, //   ####   ####   
	0b01111110, 0b11111100, //  ###### ######  
	0b11111111, 0b11111110, // ############### 
	0b11111111, 0b11111110, // ############### 
	0b11111111, 0b11111110, // ############### 
	0b11111111, 0b11111110, // ############### 
	0b01111111, 0b11111100, //  #############  
	0b01111111, 0b11111100, //  #############  
	0b00111111, 0b11111000, //   ###########   
	0b00011111, 0b11110000, //    #########    
	0b00001111, 0b11100000, //     #######     
	0b00000111, 0b11000000, //      #####      
	0b00000011, 0b10000000, //       ###    
	0b00000001, 0b00000000, //    	  #
};

// This is the HTML code for our webpage for submitting the messages to NodeMCU.
const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
* {
  box-sizing: border-box;
}

input[type=text], select, textarea {
  width: 100%;
  padding: 12px;
  border: 1px solid #ccc;
  border-radius: 4px;
  resize: vertical;
}

label {
  padding: 12px 12px 12px 0;
  display: inline-block;
}

input[type=submit] {
  background-color: #04AA6D;
  color: white;
  width: 100%;
  padding: 12px 20px;
  border: none;
  border-radius: 4px;
  cursor: pointer;
  float: right;
  
}

input[type=submit]:hover {
  background-color: #45a049;
}

.container {
  border-radius: 5px;
  background-color: #f2f2f2;
  padding: 20px;
}

.col-25 {
  float: left;
  width: 25%;
  margin-top: 6px;
}

.col-75 {
  float: left;
  width: 75%;
  margin-top: 6px;
}

/* Clear floats after the columns */
.row:after {
  content: "";
  display: table;
  clear: both;
}

/* Responsive layout - when the screen is less than 600px wide, make the two columns stack on top of each other instead of next to each other */
@media screen and (max-width: 600px) {
  .col-25, .col-75, input[type=submit] {
    width: 100%;
    margin-top: 0;
  }
}
</style>
</head>
<body>
<h2 align="center">RCS Academy</h2>
<p align="center">Web Based Pager</p>

<div class="container">
  <form action="/action_page">
    <div class="row">
      <div class="col-25">
        <label for="from">From</label>
      </div>
      <div class="col-75">
        <input type="text" id="from" name="from" placeholder="Your name..">
      </div>
    </div>
    <div class="row">
      <div class="col-25">
        <label for="icon">Icon</label>
      </div>
      <div class="col-75">
        <select id="icon" name="icon">
          <option value="heart_icon">Heart</option>
          <option value="warning_icon">Warning</option>
        </select>
      </div>
    </div>
    <div class="row">
      <div class="col-25">
        <label for="message">Message</label>
      </div>
      <div class="col-75">
        <textarea id="message" name="message" placeholder="Write something.." style="height:200px"></textarea>
      </div>
    </div>
    <div class="row">
      <input type="submit" value="Submit">
    </div>
  </form>
</div>

</body>
</html>

)=====";

// SSID and Password of your WiFi router
const char* ssid = "WifiName";
const char* password = "WifiPassword";

ESP8266WebServer server(80); //Server on port 80

// When you visit the URL, this function will send the HTML code above to your web browser.
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

// When you click on "Submit" on the form, following function will run.
void handleForm() {
 String from = server.arg("from"); // This line gets what you wrote in the "From" field and inserts it into the "from" variable (Data type is String).
 String message = server.arg("message"); // This line gets what you wrote in the "Message" field and inserts it into the "message" variable (Data type is String).
 String icon = server.arg("icon"); // This line gets what you select in the "Icon" dropdown box and inserts it into the "icon" variable (Data type is String).

 Serial.print("From: ");
 Serial.println(from);  // Print the "From" field on our serial console.

 Serial.print("Message: ");
 Serial.println(message);  // Print the "Message" field on our serial console.

          display.clearDisplay(); // First we must clear the display memory.
          display.setTextColor(WHITE); // Then we select the color.
          display.setCursor(0,20); // On this line, we select the pixel to begin writing. First number is the column, second is the row.
          display.setTextSize(1); // Set the font size.
          display.print(F("From:")); // Write the text "From".
          display.print(from); // Write the text we entered to the "from" variable.
          display.setCursor(0,29); // Again, we set the column and row.
          display.print("--------------------"); // This is the divider text.
          display.setCursor(0,38); // Again, we set the column and row.
          display.print(message); // Write the text we entered to the "message" variable.
          if (icon=="warning_icon"){ // If you select the warning icon from the dropdown menu, we display the Warning Icon
            display.drawBitmap(56, 0, warning_icon, 16, 16, 1);
            }else{ // If you didn't select the warning icon, we display the Heart Icon
              display.drawBitmap(56, 0, heart_icon, 16, 16, 1);
            }
          display.display(); // This is the moment all the data above is sent to the display memory.

            digitalWrite(firstLed, LOW);
            // Wait for one second, in this case, 1000 miliseconds.
            delay(1000);
            // Turn one LED on, the other LED off. HIGH means give it 3.3 volts, LOW means give it 0 volts.
            digitalWrite(firstLed, HIGH);
            digitalWrite(secondLed, LOW); // Only the CP2102 Board has this LED.
            // Wait for one second, in this case, 1000 miliseconds.
            delay(1000);
            digitalWrite(secondLed, HIGH);
 
 String s = "<center><a href='/'> <button style='FONT-SIZE: 50px; HEIGHT: 200px; WIDTH: 300px; 126px; Z-INDEX: 0; TOP: 200px;'> Go Back </button> </a></center>"; // This code is for our "Go Back" button.
 server.send(200, "text/html", s); // Send the "Go Back" page.
}

void setup(void){
  Serial.begin(115200); // Setting up the serial console.

  // Initialize both of the on-board LEDs, set respective pins as output.
  pinMode(firstLed, OUTPUT); // Instead of D4 (Digital Pin 4) we could use the word LED_BUILTIN.
  pinMode(secondLed, OUTPUT); // Only the CP2102 Board has this pin.


  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }


  WiFi.begin(ssid, password);     // Connect to your WiFi router
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println("WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  // IP address assigned to your router / modem.
  
  display.clearDisplay();
          display.setTextColor(WHITE);
          display.setCursor(10,0);
          display.setTextSize(2);
          display.print(F("Welcome!"));
          display.setTextSize(1);
          display.setCursor(5,30);
          display.print(F("Please Send Your"));
          display.setCursor(20,41);
          display.print(F("Message At"));
          display.setCursor(5,52);
          display.print(F("http://"));
          display.print(WiFi.localIP());
          display.display();

  server.on("/", handleRoot);      // This is the root folder of our webpage.
  server.on("/action_page", handleForm); // Form action is handled here.

  server.begin();                  // Start the web server.
  Serial.println("HTTP server started");
}

void loop(void){

          digitalWrite(firstLed, HIGH);
          digitalWrite(secondLed, HIGH);

  server.handleClient();          // We will only watch if somebody visits the page.

}
