#include <SPI.h>
#include <WiFi.h>

#define ARDUINO_PLATFORM
#include "M2XStreamClient.h"

char ssid[] = "<ssid>"; //  your network SSID (name)
char pass[] = "<WPA password>";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

char* deviceId = "<device id>"; // Device you want to push to
char* streamName = "<stream name>"; // Stream you want to push to
char* m2xKey = "<M2X access key>"; // Your M2X access key

char timestamp[25];

const int temperaturePin = 0;
WiFiClient client;
M2XStreamClient m2xClient(&client, m2xKey);
TimeService timeService(&m2xClient);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while(true);
  }

  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();

  if (!m2x_status_is_success(timeService.init())) {
    Serial.println("Cannot initialize time service!");
    while(1) {}
  }
}

void loop() {
  float voltage, degreesC, degreesF;

  voltage = getVoltage(temperaturePin);
  degreesC = (voltage - 0.5) * 100.0;
  degreesF = degreesC * (9.0/5.0) + 32.0;

  Serial.print("voltage: ");
  Serial.print(voltage);
  Serial.print("  deg C: ");
  Serial.print(degreesC);
  Serial.print("  deg F: ");
  Serial.println(degreesF);

  int length = 25;
  timeService.getTimestamp(timestamp, &length);

  Serial.print("Current timestamp: ");
  Serial.println(timestamp);

  char *timestamps[1];
  timestamps[0] = timestamp;

  int count = 1;
  int response = m2xClient.postDeviceUpdates<float>(
      deviceId, 1, (const char **) &streamName,
      &count, (const char **) timestamps, &degreesC);
  Serial.print("M2x client response code: ");
  Serial.println(response);

  if (response == -1) while(1) ;

  delay(5000);
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

float getVoltage(int pin)
{
  return (analogRead(pin) * 0.004882814);
}
