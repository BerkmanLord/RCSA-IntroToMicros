/* RGB LED web server controller with ESP8266
* analogWrite with values received from the sliders from the web page
*/

#include <ESP8266WebServer.h>
#include <Arduino.h>

const char* ssid = "WifiName";
const char* password = "WifiPassword";

ESP8266WebServer server(80);

const int red = D7; // Red LED Pin
const int green = D6; // Green LED Pin
const int blue = D5; // Blue LED Pin

String page =
R"(
<html lang='en'>
    <head>
        <title>Dim an RGB LED</title>
        <meta name='viewport' content='width=device-width, initial-scale=1'>
        <link rel='stylesheet' href='https://use.fontawesome.com/releases/v5.1.0/css/all.css'/>
        <link rel='stylesheet' href='https://stackpath.bootstrapcdn.com/bootstrap/4.1.2/css/bootstrap.min.css'/>
        <script src='https://code.jquery.com/jquery-3.3.1.min.js'></script>
    </head>
    <body>
    <div class='container-fluid text-center'>
        <div class='row'>
            <div class='col-sm-12' style='color:black;font-size:4vh'>
                <p>RCS Academy</p>
            </div>
        </div>     
        <br/>
               <div class='row'>
            <div class='col-sm-12' style='color:black;font-size:2vh'>
                <p>RGB Color Mixer</p>
            </div>
        </div>     
        <br/>
        <div class='row'>
            <div class='col-sm-12' style='color:red;font-size:3vh'>
                Red:
                <b id='status1'></b>
            </div>
        </div> 
        <div class='col-sm-12'>
              <input type='range' min='0' max='100' class='form-control-range' id='slider_red'/>
        </div>

        <div class='row'>
            <div class='col-sm-12' style='color:green;font-size:3vh'>
                Green: 
                <b id='status2'></b>
            </div>
        </div> 
        <div class='col-sm-12'>
              <input type='range' min='0' max='100' class='form-control-range' id='slider_green'/>
        </div>

        <div class='row'>
            <div class='col-sm-12' style='color:blue;font-size:3vh'>
                Blue:
                <b id='status3'></b>
            </div>
        </div> 
        <div class='col-sm-12'>
              <input type='range' min='0' max='100' class='form-control-range' id='slider_blue'/>
        </div>
    </div>
    </body>
    
    <script>
        $('#slider_red').on('change', function(){
            var state_slider_red = $('#slider_red').val();
            
            $('#status1').html(state_slider_red + ' %');

            $.ajax({
              url: '/redFunction',
              type: 'POST',
              data: {state1: state_slider_red}
            });
        });

        $('#slider_green').on('change', function(){
            var state_slider_green = $('#slider_green').val();
            
            $('#status2').html(state_slider_green + ' %');

            $.ajax({
              url: '/greenFunction',
              type: 'POST',
              data: {state2: state_slider_green}
            });
        });

        $('#slider_blue').on('change', function(){
            var state_slider_blue = $('#slider_blue').val();
            
            $('#status3').html(state_slider_blue + ' %');

            $.ajax({
              url: '/blueFunction',
              type: 'POST',
              data: {state3: state_slider_blue}
            });
        });
    </script>
</html>
)";

void redFunction()
{
  int value1 = 100 - server.arg("state1").toInt();
  value1 = map(value1, 0, 100, 0, 255);

     analogWrite(red, value1);//change the brightness of red
  
  server.send(200, "text/html", "red");
}

void greenFunction()
{
  int value2 = 100 - server.arg("state2").toInt();
  value2 = map(value2, 0, 100, 0, 255);

     analogWrite(green, value2);//change the brightness of green
  
  server.send(200, "text/html", "green");
}

void blueFunction()
{
  int value3 = 100 - server.arg("state3").toInt();
  value3 = map(value3, 0, 100, 0, 255);

     analogWrite(blue, value3);//change the brightness of blue
  
  server.send(200, "text/html", "blue");
}

void htmlIndex() 
{
 
  server.send(200, "text/html", page);
}


void connectToWiFi()
{ 
  Serial.println("Connecting to the WiFi");
  
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  Serial.println("Waiting for connection");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


void setupServer()
{ 

  server.on("/", htmlIndex);
  server.on("/redFunction", redFunction);
  server.on("/greenFunction", greenFunction);
  server.on("/blueFunction", blueFunction);

  server.begin();
  
  Serial.println("HTTP server started");
}

void setup() 
{
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);

  Serial.begin(115200);

  delay(1000);

  connectToWiFi();
  setupServer();
}

void loop()
{
  
  server.handleClient();
}
