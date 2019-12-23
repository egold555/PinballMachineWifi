#include "NTPClient.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>

const char *ssid = "EricDebug";
const char *password = "debug1234";

const long utcOffsetInSeconds = -28800;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

WiFiServer server(80);

void setup()
{
  Serial.begin(115200);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

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

  Serial.println("Time Client started");
  timeClient.begin();
}

void loop()
{
  timeClient.update();

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available())
  {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("Connection: close");
  client.println(); //  do not forget this one
  client.println("<!DOCTYPE HTML>");

  client.println("<html>");
  client.println("<head>");
  client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");
  client.println("</head>");
  client.println("<body>");

  client.print(timeClient.getFormattedDay() + "<br>");
  client.print(timeClient.getFormattedTime12() + "<br>");
  client.print(timeClient.getFormattedDate());

  client.println("</body>");
  client.println("</html>");

  client.stop();
  Serial.println("Client disonnected");
  Serial.println("");
}
