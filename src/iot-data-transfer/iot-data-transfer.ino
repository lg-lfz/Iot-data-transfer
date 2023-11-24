#include <cstring>
#include <ESP8266WiFiMulti.h>
#include <ESPAsyncTCP.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>

ESP8266WiFiMulti wifiMulti;
// Influx DB Cloud Settings
#define INFLUXDB_URL "https://eu-central-1-1.aws.cloud2.influxdata.com"
#define INFLUXDB_TOKEN ""
#define INFLUXDB_ORG ""
#define INFLUXDB_BUCKET "temperature"
// Time zone info
// https://www.gnu.org/software/libc/manual/html_node/TZ-Variable.html
// Examples:
// https://gist.github.com/alwynallan/24d96091655391107939
//  Pacific Time: "PST8PDT"
//  Eastern: "EST5EDT"
//  Japanesse: "JST-9"
//  Central Europe: "CET-1CEST,M3.5.0,M10.5.0/3"
#define TZ_INFO "CET-1CEST,M3.5.0,M10.5.0/3"
// Declare InfluxDB client instance with preconfigured InfluxCloud certificate
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

// Declare Data point
Point sensor("Temperature");

// WIFI Settings
constexpr const char *wifi_ssid = "";
constexpr const char *wifi_psk = "";
constexpr float board_voltage = 3.3; // Spannung an den ESP8266 Pins

// TMP36 Pin Variable
int sensorPin = A0; // Analoger Eingangspin, an welchem der TMP36 angeschlossen ist
                    // Die Auflösung beträgt 10 mV / Grad Celsius bei
                    // 500 mV Offset um das Messen negativer Temperaturen zu ermöglichen
bool led_on = true;

void setup()
{
  Serial.begin(115200);         // Start des seriellen Monitors
  
  // Add tags to the data point
  sensor.addTag("device", "ESP8266");
  sensor.addTag("room", "Office");
  sensor.addTag("sensor", "TMP36GZ");
  
  pinMode(LED_BUILTIN, OUTPUT); // Initialize the LED_BUILTIN pin as an output
  connect_to_wifi();
}

void loop()
{
  int reading = analogRead(sensorPin); // Den Signalausgang des Temperatursensors lesen
  Serial.print("Raw value (0-1023): ");
  Serial.println(reading);
  float voltage = reading * board_voltage; // Umwandlung der Messung in mV
  voltage /= 1024.0;

  Serial.print("Gemessene Spannung: ");
  Serial.print(voltage); // Ausgabe der gemessenen Spannung in mV
  Serial.print(" mV;\t");
  float temperatureC = (voltage - 0.5) * 100; // Umrechnung der Spannung in C°

  Serial.print("Gemessene Temperatur: ");
  Serial.print(temperatureC); // Ausgabe der berechneten Temperatur in °C
  Serial.println(" °C");

  // Clear fields for reusing the point. Tags will remain the same as set above.
  sensor.clearFields();
  
  // Store measured value into point
  // Report temperature
  sensor.addField("temperature", temperatureC);
  
  // Print what are we exactly writing
  Serial.print("Schreibe Daten nach ");
  Serial.println(client.getServerUrl());
  Serial.println(sensor.toLineProtocol());
  
  // Check WiFi connection and reconnect if needed
  if (wifiMulti.run() != WL_CONNECTED) {
    Serial.println("Wifi connection lost");
  }
  
  // Write point
  if (!client.writePoint(sensor)) {
    Serial.print("InfluxDB write failed: ");
    Serial.println(client.getLastErrorMessage());
  }
  
  // Show time
  time_t tnow = time(nullptr);
  Serial.print("Zeit jetzt: ");
  Serial.println(ctime(&tnow));

  if (led_on)
  {
    digitalWrite(LED_BUILTIN, LOW); // Turn the LED on by making the voltage LOW
  }
  else
  {
    digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off by making the voltage HIGH
  }
  led_on = !led_on;
  delay(1000); // 1 Sekunde warten bis zur nächsten Messung
}

void connect_to_wifi()
{
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(wifi_ssid, wifi_psk);
  
  Serial.print("Connecting to wifi");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  
  // Accurate time is necessary for certificate validation and writing in batches
  // We use the NTP servers in your area as provided by: https://www.pool.ntp.org/zone/
  // Syncing progress and the time will be printed to Serial.
  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Check server connection
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }
}