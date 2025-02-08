#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
WebServer server(80);

#define SENSOR1  13
#define SENSOR2  14

const char* ssid = "";
const char* password = "";

long currentMillis1 = 0;
long previousMillis1 = 0;
int interval1 = 1000;
float calibrationFactor1 = 6;
volatile byte pulseCount1;
byte pulse1Sec1 = 0;
float flowRate1;
unsigned int flowMilliLitres1;
unsigned long totalMilliLitres1;

void IRAM_ATTR pulseCounter1() {
  pulseCount1++;
}

long currentMillis2 = 0;
long previousMillis2 = 0;
int interval2 = 1000;
float calibrationFactor2 = 6;
volatile byte pulseCount2;
byte pulse1Sec2 = 0;
float flowRate2;
unsigned int flowMilliLitres2;
unsigned long totalMilliLitres2;

void IRAM_ATTR pulseCounter2() {
  pulseCount2++;
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("connecting..");
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.begin();

  pinMode(SENSOR1, INPUT_PULLUP);
  pinMode(SENSOR2, INPUT_PULLUP);
  
  pulseCount1 = 0;
  pulseCount2 = 0;
  flowRate1 = 0.0;
  flowRate2 = 0.0;
  totalMilliLitres1 = 0;
  totalMilliLitres2 = 0;
  
  attachInterrupt(digitalPinToInterrupt(SENSOR1), pulseCounter1, FALLING);
  attachInterrupt(digitalPinToInterrupt(SENSOR2), pulseCounter2, FALLING);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1, 0);
  lcd.print("IoT Water Flow");
  lcd.setCursor(1, 1);
  lcd.print("Monitoring");
  delay(2000);
}

void loop() {
  server.handleClient();

  currentMillis1 = millis();
  if (currentMillis1 - previousMillis1 > interval1) {
    pulse1Sec1 = pulseCount1;
    pulseCount1 = 0;
    flowRate1 = ((1000.0 / (millis() - previousMillis1)) * pulse1Sec1) / calibrationFactor1;
    previousMillis1 = millis();
    flowMilliLitres1 = (flowRate1 / 60) * 1000;
    totalMilliLitres1 += flowMilliLitres1;

    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Flow1=");
    lcd.print(int(flowRate1));
    lcd.print(" mL/S");

    Serial.print("Flow Rate 1: ");
    Serial.print(flowRate1);
    Serial.println(" mL/S");
  }

  currentMillis2 = millis();
  if (currentMillis2 - previousMillis2 > interval2) {
    pulse1Sec2 = pulseCount2;
    pulseCount2 = 0;
    flowRate2 = ((1000.0 / (millis() - previousMillis2)) * pulse1Sec2) / calibrationFactor2;
    previousMillis2 = millis();
    flowMilliLitres2 = (flowRate2 / 60) * 1000;
    totalMilliLitres2 += flowMilliLitres2;

    lcd.setCursor(1, 1);
    lcd.print("Flow2=");
    lcd.print(int(flowRate2));
    lcd.print(" mL/S");

    Serial.print("Flow Rate 2: ");
    Serial.print(flowRate2);
    Serial.println(" mL/S");
  }
}

void handleRoot() {
  String html = "<html><head><title>Water Flow Monitor</title>";
  html += "<meta http-equiv='refresh' content='3'>";
  html += "</head><body>";
  html += "<h1>Water Flow Data</h1>";
  html += "<p>Flow Rate 1: " + String(flowRate1) + " mL/S</p>";
  html += "<p>Flow Rate 2: " + String(flowRate2) + " mL/S</p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}
