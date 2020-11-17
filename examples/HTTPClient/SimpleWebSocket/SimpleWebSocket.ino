/****************************************************************************************************************************
  SimpleWebSocket.ino - Simple Arduino web server sample for WiFi shield

  For any WiFi shields, such as WiFiNINA W101, W102, W13x, or custom, such as ESP8266/ESP32-AT, Ethernet, etc
  
  WiFiWebServer is a library for the ESP32-based WiFi shields to run WebServer
  Based on and modified from ESP8266 https://github.com/esp8266/Arduino/releases
  Based on  and modified from Arduino WiFiNINA library https://www.arduino.cc/en/Reference/WiFiNINA
  Built by Khoi Hoang https://github.com/khoih-prog/WiFiWebServer
  Licensed under MIT license

  Simple WebSocket client for HttpClient
  Connects to the WebSocket server, and sends a hello
  message every 5 seconds

  created 28 Jun 2016
  by Sandeep Mistry
  modified 22 Jan 2019
  by Tom Igoe
 *****************************************************************************************************************************/

#include "defines.h"

char serverAddress[] = "echo.websocket.org";  // server address
int port = 80;

WiFiClient           client;
WiFiWebSocketClient  wsClient(client, serverAddress, port);

int count = 0;

int status = WL_IDLE_STATUS;     // the Wifi radio's status

void printWifiStatus()
{
  // print the SSID of the network you're attached to:
  // you're connected now, so print out the data
  Serial.print(F("You're connected to the network, IP = "));
  Serial.println(WiFi.localIP());

  Serial.print(F("SSID: "));
  Serial.print(WiFi.SSID());

  // print the received signal strength:
  int32_t rssi = WiFi.RSSI();
  Serial.print(F(", Signal strength (RSSI):"));
  Serial.print(rssi);
  Serial.println(F(" dBm"));
}

void setup()
{
  Serial.begin(115200);
  while (!Serial);

  Serial.print("\nStarting SimpleWebSocket on " + String(BOARD_NAME));
  Serial.println(" with " + String(SHIELD_TYPE));

  // check for the presence of the shield
#if USE_WIFI_NINA
  if (WiFi.status() == WL_NO_MODULE)
#else
  if (WiFi.status() == WL_NO_SHIELD)
#endif
  {
    Serial.println(F("WiFi shield not present"));
    // don't continue
    while (true);
  }

#if USE_WIFI_NINA
  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION)
  {
    Serial.println(F("Please upgrade the firmware"));
  }
#endif

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED)
  {
    Serial.print(F("Connecting to SSID: "));
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  printWifiStatus();
}

void loop()
{
  Serial.println("starting WebSocket client");
  
  wsClient.begin();

  while (wsClient.connected()) 
  {
    Serial.print("Sending Hello ");
    Serial.println(count);

    // send a hello #
    wsClient.beginMessage(TYPE_TEXT);
    wsClient.print(count);
    String data = " => Hello from SimpleWebSocket on " + String(BOARD_NAME) + ", millis = " + String(millis());
    wsClient.print(data);
    wsClient.endMessage();

    // increment count for next message
    count++;

    // check if a message is available to be received
    int messageSize = wsClient.parseMessage();

    if (messageSize > 0) 
    {
      Serial.println("Received a message:");
      Serial.println(wsClient.readString());
    }

    // wait 5 seconds
    delay(5000);
  }

  Serial.println("disconnected");
}
