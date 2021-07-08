/*
 * LED Control - RCS Academy - Lord
 * Software for our Web Controlled LED
 * You can connect a motor to turn it on or off!
 * You can edit the webpage to make it look better, or add custom text!
 * You can set the WIFI details to connect to another network.
 */


#include <Arduino.h>
#include <ESP8266WiFi.h> //This is our library to do things with the internet & WIFI.

 
// Between the " signs, we write our WIFI name, also known as SSID. Like "Lord's Wifi".
const char* ssid = "";
// Between the " signs, we write our WIFI password. Like "SuperSecret".
const char* password = "";

WiFiServer server(80); // This is to start our webserver. On network port "80".


void setup() {

  Serial.begin(115200);  // Starting our serial port, so we can have debug information.

  pinMode(LED_BUILTIN, OUTPUT); // We are setting our built-in LED as an output, like the last time.
  digitalWrite(LED_BUILTIN, LOW); // Turning the pin off, to get our device ready to use.


  pinMode(D0, OUTPUT); // We are setting our built-in LED as an output, like the last time.
  digitalWrite(D0, LOW); // Turning the pin off, to get our device ready to use.


  // Connect to WiFi network
  Serial.print("Connecting to "); // Writing this to our serial console.
  Serial.println(ssid); // This line will write the WIFI name to the serial console.
  WiFi.begin(ssid, password); // This line connects us to the WIFI network we wrote above.
 

  while (WiFi.status() != WL_CONNECTED) {

    delay(500);

    Serial.print("."); // This will add dots to our serial console while connecting, like a progress / loading bar.

  }

  Serial.println("");
  Serial.println("WiFi connected"); // Once connected, we will write to serial console that WIFI has been connected.

 

  // Start the server
  server.begin();
  Serial.println("Server started at..."); // Our modem will give us an IP address, where our Web Server will start at.
  Serial.println(WiFi.localIP()); // This line will write our IP address to the serial console.

}

 

void loop() {

  // The Client is you! You become a client when you connect to the IP address displayed on the serial monitor.
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

 

  // Our device will recognize us as connected when we load in or click a button on the web page.
  Serial.println("New Connection!");
  while (! client.available())
  {
    delay (1);
  }


  // Read the first line of the request. Request is the button you press! Turn on or off!
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  // Grant the request! Check the URL at the top when you click a button!
  if (req.indexOf("/led1off") != -1)  { //If URL is /ledoff then,
    digitalWrite(LED_BUILTIN, HIGH); // You set the pin High! Because we are controlling the potential difference! This is the a new concept! Sometimes, we can turn things off when we set the pin HIGH!
    Serial.println("LED OFF"); // Write on the serial console that our LED is OFF.
  }

  else if(req.indexOf("/led1on") != -1) //If URL is /ledon then,

  {
    digitalWrite(LED_BUILTIN, LOW); // Setting the pin LOW and making the potential difference high! Sometimes we can turn things on when we set the pin LOW!
    Serial.println("LED ON"); // Write on the serial console that our LED is ON.
  }

    // Grant the request! Check the URL at the top when you click a button!
  if (req.indexOf("/led2off") != -1)  { //If URL is /ledoff then,
    digitalWrite(D0, HIGH); // You set the pin High! Because we are controlling the potential difference! This is the a new concept! Sometimes, we can turn things off when we set the pin HIGH!
    Serial.println("LED OFF"); // Write on the serial console that our LED is OFF.
  }

  else if(req.indexOf("/led2on") != -1) //If URL is /ledon then,

  {
    digitalWrite(D0, LOW); // Setting the pin LOW and making the potential difference high! Sometimes we can turn things on when we set the pin LOW!
    Serial.println("LED ON"); // Write on the serial console that our LED is ON.
  }

 

// Here is our webpage code!

client.println("HTTP/1.1 200 OK");

client.println("Content-Type: text/html");

client.println("Connection: close");

client.println("");


client.println("<!DOCTYPE HTML>");

client.println("<HTML>");

client.println("<CENTER><img src='https://rcsplus.org/en/wp-content/uploads/2018/12/cropped-rcsa-logo.png'/>"); // You can add your own image address!
client.println("<H1> RCS Academy LED Control </H1>"); // You can actually change the text on the page!

client.println("<br />");

 

 client.println("<a href=\"/led1on\"\"> <button style='FONT-SIZE: 50px; HEIGHT: 200px;WIDTH: 300px; 126px; Z-INDEX: 0; TOP: 200px;'> Turn On </button> </a>"); // Button to turn our LED on.

 client.println("<a href=\"/led1off\"\"> <button style='COLOR: red; FONT-SIZE: 50px; HEIGHT: 200px; WIDTH: 300px; 126px; Z-INDEX: 0; TOP: 200px;'> Turn Off </button> </a>"); // Button to turn our LED off.

 client.println("<a href=\"/led2on\"\"> <button style='FONT-SIZE: 50px; HEIGHT: 200px;WIDTH: 300px; 126px; Z-INDEX: 0; TOP: 200px;'> Turn On </button> </a>"); // Button to turn our LED on.

 client.println("<a href=\"/led2off\"\"> <button style='COLOR: red; FONT-SIZE: 50px; HEIGHT: 200px; WIDTH: 300px; 126px; Z-INDEX: 0; TOP: 200px;'> Turn Off </button> </a>"); // Button to turn our LED off.


 client.println("</CENTER></html>");

 

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
