/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "ESPap";
const char *password = "securedpassword";

/* Go to http://192.168.4.1 in a web browser
 * connected to this access point.
 */
ESP8266WebServer server(80);

bool blinking = true;
int speed = 1000;
const int ledPin =  LED_BUILTIN;
int ledState = HIGH;
unsigned long previousMillis = 0;

String mainPage = "<h1>ESP8266 Web Server</h1><p>LED <a href=\"led?state=on\"><button>ON</button></a>&nbsp;<a href=\"led?state=off\"><button>OFF</button></a>&nbsp;<a href=\"blink?delay=1000\"><button>BLINK</button></a></p>";

void showMainPage() {
	server.send(200, "text/html", mainPage);
}

void handleLed(){
  blinking = false;
  if(server.hasArg("state")){
      String state = server.arg("state");
      if(state == "on"){
        digitalWrite(ledPin, LOW);
      } else if (state == "off"){
        digitalWrite(ledPin, HIGH);
      }
  }
  showMainPage();
}

void handleBlink(){
  if(server.hasArg("delay")){
      int delay = server.arg("delay").toInt();
      if(delay >= 10 && delay <= 5000){
        speed = delay;
      }
  }
  blinking = true;
  showMainPage();
}

void setup() {
	 delay(1000);
   pinMode(LED_BUILTIN, OUTPUT);
   digitalWrite(LED_BUILTIN, HIGH);

	/* You can remove the password parameter if you want the AP to be open. */
	WiFi.softAP(ssid, password);

	IPAddress myIP = WiFi.softAPIP();
	//Serial.print("AP IP address: ");
	//Serial.println(myIP);
	
	server.on("/", showMainPage);
  server.on("/led", handleLed);
  server.on("/blink", handleBlink);

	server.begin();
	//Serial.println("HTTP server started");
}

void loop() {
	server.handleClient();

  unsigned long currentMillis = millis();

  if (blinking && currentMillis - previousMillis >= speed) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
  
}
