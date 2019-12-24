#include "NTPClient.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>   // Include the WebServer library

const char *ssid = "EricDebug";
const char *password = "debug1234";

const long utcOffsetInSeconds = -28800;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

ESP8266WebServer server(80);

void setup()
{
  Serial.begin(115200);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA); //no need to broadcast a wifi access point

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

  server.on("/", handlePinballTable);               // Call the 'handleRoot' function when a client requests URI "/"
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call 

  Serial.println("Time Client started");
  timeClient.begin();
}

void loop()
{
  timeClient.update();
  server.handleClient();                    // Listen for HTTP requests from clients
}

void handleRoot() {
  String data = timeClient.getFormattedDay() + "<br>" + timeClient.getFormattedTime12() + "<br>" + timeClient.getFormattedDate();
  server.send(200, "text/html", data);   // Send HTTP status 200 (Ok) and send some text to the browser/client
}

const String headerPinball = "<!DOCTYPE html><html lang=\"en\" ><head><meta charset=\"UTF-8\"><title>Pinball Highscores</title><style>table{font-family:arial,sans-serif;border-collapse:collapse;width:100%}td,th{border:1px solid #ddd;text-align:left;padding:8px}tr:nth-child(even){background-color:#ddd}</style></head><body><table><tr><th>Name</th><th>Score</th><th>Date</th></tr>";
const String footerPinball = "</table></body></html>";

String genRow(String name, int score, String fdt) {
  return "<tr><td>" + name + "</td><td>" + score + "</td><td>" + fdt + "</td></tr>";
}

void handlePinballTable() {

  String data = headerPinball;
  data += genRow("Eric", 12345, timeClient.getFormattedDay() + " " + timeClient.getFormattedTime12() + " " + timeClient.getFormattedDate());
  data += genRow("Joe", 54567, timeClient.getFormattedDay() + " " + timeClient.getFormattedTime12() + " " + timeClient.getFormattedDate());
  data += genRow("Billy", 92384, timeClient.getFormattedDay() + " " + timeClient.getFormattedTime12() + " " + timeClient.getFormattedDate());
  data += genRow("Pat", 0, timeClient.getFormattedDay() + " " + timeClient.getFormattedTime12() + " " + timeClient.getFormattedDate());
  data += genRow("Hannah", 434, timeClient.getFormattedDay() + " " + timeClient.getFormattedTime12() + " " + timeClient.getFormattedDate());
  data += genRow("Emma", 294, timeClient.getFormattedDay() + " " + timeClient.getFormattedTime12() + " " + timeClient.getFormattedDate());
  data+= footerPinball;

  server.send(200, "text/html", data);   // Send HTTP status 200 (Ok) and send some text to the browser/client
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}