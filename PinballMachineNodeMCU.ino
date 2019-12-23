#include "NTPClient.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char *ssid     = "EricDebug";
const char *password = "debug1234";

const long utcOffsetInSeconds = -28800;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup(){
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
}

void loop() {
  timeClient.update();

  Serial.println(timeClient.getFormattedDay());
  Serial.println(timeClient.getFormattedTime12());
  Serial.println(timeClient.getFormattedDate());
  Serial.println();

  delay(1000);
}
//#include <ESP8266WiFi.h>
// 
//const char* ssid = "EricDebug";
//const char* password = "debug1234";
// 
//WiFiServer server(80);
// 
//void setup() {
//  Serial.begin(57600);
//  delay(10);
// 
//  pinMode(LED_BUILTIN, OUTPUT);
//  digitalWrite(LED_BUILTIN, LOW);
// 
//  // Connect to WiFi network
//  Serial.println();
//  Serial.println();
//  Serial.print("Connecting to ");
//  Serial.println(ssid);
// 
//  WiFi.begin(ssid, password);
// 
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//  Serial.println("");
//  Serial.println("WiFi connected");
// 
//  // Start the server
//  server.begin();
//  Serial.println("Server started");
// 
//  // Print the IP address
//  Serial.print("Use this URL to connect: ");
//  Serial.print("http://");
//  Serial.print(WiFi.localIP());
//  Serial.println("/");
// 
//}
// 
//void loop() {
//  // Check if a client has connected
//  WiFiClient client = server.available();
//  if (!client) {
//    return;
//  }
// 
//  // Wait until the client sends some data
//  Serial.println("new client");
//  while(!client.available()){
//    delay(1);
//  }
// 
//  // Read the first line of the request
//  String request = client.readStringUntil('\r');
//  Serial.println(request);
//  client.flush();
// 
//  // Match the request
// 
//  int value = LOW;
//  if (request.indexOf("/LED=ON") != -1)  {
//    digitalWrite(LED_BUILTIN, LOW);
//    value = LOW;
//  }
//  if (request.indexOf("/LED=OFF") != -1)  {
//    digitalWrite(LED_BUILTIN, HIGH);
//    value = HIGH;
//  }
// 
//// Set LED_BUILTIN according to the request
////digitalWrite(LED_BUILTIN, value);
// 
//  // Return the response
//  client.println("HTTP/1.1 200 OK");
//  client.println("Content-Type: text/html");
//  client.println(""); //  do not forget this one
//  client.println("<!DOCTYPE HTML>");
//  client.println("<html>");
// 
//  client.print("Led pin is now: ");
// 
//  if(value == HIGH) {
//    client.print("On");
//  } else {
//    client.print("Off");
//  }
//  client.println("<br><br>");
//  client.println("<a href=\"/LED=ON\"\"><button>Turn On </button></a>");
//  client.println("<a href=\"/LED=OFF\"\"><button>Turn Off </button></a><br />");  
//  client.println("</html>");
// 
//  delay(1);
//  Serial.println("Client disonnected");
//  Serial.println("");
// 
//}
 
