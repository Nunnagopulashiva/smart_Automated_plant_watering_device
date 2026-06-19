#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>

#define SOIL_SENSOR 34
#define RELAY_PIN 26

// WiFi Credentials
const char* ssid = "ZTE-3PdEt3";
const char* password = "jrkgs3g9";
// Flask server running on laptop
String server = "http://192.168.1.5:5000/update";

// Soil dryness threshold
int threshold = 2300;
int moistureValue;

// Time settings for India
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 19800;      // UTC +5:30
const int daylightOffset_sec = 0;

void setup() {

  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);

  // Relay OFF initially
  digitalWrite(RELAY_PIN, HIGH);

  Serial.println("Connecting WiFi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting WiFi...");
  }

  Serial.println("WiFi Connected");
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());

  // Get internet time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  Serial.println("--------------------------------");
}

void loop() {

  // Read soil moisture sensor continuously
  moistureValue = analogRead(SOIL_SENSOR);

  Serial.print("Moisture: ");
  Serial.println(moistureValue);

  // Default pump status
  String pumpStatus = "OFF";

  // Get current time
  struct tm timeinfo;
  bool allowedTime = false;

  if (getLocalTime(&timeinfo)) {

    int currentHour = timeinfo.tm_hour;
    int currentMinute = timeinfo.tm_min;

    Serial.print("Current Time: ");

    if (currentHour < 10) Serial.print("0");
    Serial.print(currentHour);
    Serial.print(":");

    if (currentMinute < 10) Serial.print("0");
    Serial.println(currentMinute);

    // Watering allowed only:
    // 06:00 AM to 06:09 AM
    // 05:00 PM to 05:09 PM
    if ((currentHour == 8  && currentMinute >= 0 && currentMinute < 50) ||
        (currentHour == 20 && currentMinute >= 0 && currentMinute < 50)) {

      allowedTime = true;
      Serial.println("Watering Time Slot Active");

    } else {

      Serial.println("Outside Watering Time");
    }

  } else {

    Serial.println("Failed to get current time");
  }

  // Pump control
  if (moistureValue > threshold && allowedTime) {

    digitalWrite(RELAY_PIN, LOW);   // Relay ON
    pumpStatus = "ON";

    Serial.println("Soil Dry - Pump ON");

  } else {

    digitalWrite(RELAY_PIN, HIGH);  // Relay OFF
    pumpStatus = "OFF";

    Serial.println("Soil Wet - Pump OFF");
  }

  // Send data silently to Flask server
  if (WiFi.status() == WL_CONNECTED) {

    HTTPClient http;

    String url = server +
                 "?value=" + String(moistureValue) +
                 "&pump=" + pumpStatus;

    http.begin(url);
    http.GET();
    http.end();

  } else {

    Serial.println("WiFi Disconnected");
  }

  Serial.println("--------------------------------");

  delay(2000);
}