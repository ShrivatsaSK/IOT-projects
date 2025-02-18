#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <LiquidCrystal_I2C.h>
#include <FS.h>
#include <SPIFFS.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
WebServer server(80);

#define SENSOR1  13
#define SENSOR2  14

const char* ssid = "";
const char* password = "";


long previousMillis1 = 0, previousMillis2 = 0;
int interval = 1000;
float calibrationFactor1 = 6, calibrationFactor2 = 6;
volatile byte pulseCount1, pulseCount2;
float flowRate1, flowRate2;
float difference;
unsigned int flowMilliLitres1, flowMilliLitres2;
unsigned long totalMilliLitres1, totalMilliLitres2;

void IRAM_ATTR pulseCounter1() { pulseCount1++; }
void IRAM_ATTR pulseCounter2() { pulseCount2++; }

void setup() {
    Serial.begin(115200);
    
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected");
    Serial.println(WiFi.localIP());

    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    server.on("/", handleRoot);
    server.on("/data", handleData);
    server.on("/download", handleDownload);
    server.begin();

    pinMode(SENSOR1, INPUT_PULLUP);
    pinMode(SENSOR2, INPUT_PULLUP);
    
    attachInterrupt(digitalPinToInterrupt(SENSOR1), pulseCounter1, FALLING);
    attachInterrupt(digitalPinToInterrupt(SENSOR2), pulseCounter2, FALLING);
    
    lcd.init();
    lcd.backlight();
    lcd.print("Water Flow Monitor");
}

void loop() {
    server.handleClient();
    
    long currentMillis = millis();
    if (currentMillis - previousMillis1 > interval) {
        flowRate1 = ((1000.0 / (millis() - previousMillis1)) * pulseCount1) / calibrationFactor1;
        flowRate2 = ((1000.0 / (millis() - previousMillis2)) * pulseCount2) / calibrationFactor2;
        difference = abs(flowRate1 - flowRate2);

        previousMillis1 = previousMillis2 = millis();
        pulseCount1 = pulseCount2 = 0;

        Serial.printf("Flow1: %.2f, Flow2: %.2f, Diff: %.2f\n", flowRate1, flowRate2, difference);

        File file = SPIFFS.open("/flow_data.csv", FILE_APPEND);
        if (file) {
            file.printf("%lu,%.2f,%.2f,%.2f\n", millis()/1000, flowRate1, flowRate2, difference);
            file.close();
        }

        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.printf("F1: %.2f mL/S", flowRate1);
        lcd.setCursor(0, 1);
        lcd.printf("F2: %.2f mL/S", flowRate2);
    }
}

void handleRoot() {
    String html = "<html><head><title>Water Flow</title>";
    html += "<script src='https://cdn.jsdelivr.net/npm/chart.js'></script>";
    
    // Auto-refresh script
    html += "<script>";
    html += "let dataFlow1=[], dataFlow2=[], dataDiff=[], labels=[];";
    html += "async function updateData() {";
    html += "let res = await fetch('/data'); let json = await res.json();";
    html += "document.getElementById('flow1').innerText = json.flow1;";
    html += "document.getElementById('flow2').innerText = json.flow2;";
    html += "document.getElementById('diff').innerText = json.difference;";
    
    html += "labels.push(json.time);";
    html += "dataFlow1.push(json.flow1);";
    html += "dataFlow2.push(json.flow2);";
    html += "dataDiff.push(json.difference);";
    
    html += "if (labels.length > 20) { labels.shift(); dataFlow1.shift(); dataFlow2.shift(); dataDiff.shift(); }";
    
    html += "chart.update();";
    html += "setTimeout(updateData, 2000);";
    html += "}";
    html += "window.onload = updateData;";
    html += "</script>";
    
    html += "</head><body>";
    html += "<h1>Water Flow Monitoring</h1>";
    
    // Display real-time values
    html += "<h3>Flow Rate 1: <span id='flow1'>0</span> mL/s</h3>";
    html += "<h3>Flow Rate 2: <span id='flow2'>0</span> mL/s</h3>";
    html += "<h3>Difference: <span id='diff'>0</span> mL/s</h3>";

    // Graph
    html += "<canvas id='chart'></canvas>";

    // Download Button
    html += "<a href='/download' download='flow_data.csv'><button>Download CSV</button></a>";

    // Chart.js Script
    html += "<script>";
    html += "let ctx = document.getElementById('chart').getContext('2d');";
    html += "let chart = new Chart(ctx, {type: 'line', data: {";
    html += "labels: labels, datasets: [";
    html += "{label: 'Flow1', data: dataFlow1, borderColor: 'blue', fill: false},";
    html += "{label: 'Flow2', data: dataFlow2, borderColor: 'green', fill: false},";
    html += "{label: 'Difference', data: dataDiff, borderColor: 'red', fill: false}]},";
    html += "options: {scales: {x: {beginAtZero: true}, y: {beginAtZero: true}}}});";
    html += "</script></body></html>";

    server.send(200, "text/html", html);
}

void handleData() {
    String json = "{";
    json += "\"time\":\"" + String(millis() / 1000) + "\",";
    json += "\"flow1\":" + String(flowRate1) + ",";
    json += "\"flow2\":" + String(flowRate2) + ",";
    json += "\"difference\":" + String(difference);
    json += "}";
    server.send(200, "application/json", json);
}

void handleDownload() {
    File file = SPIFFS.open("/flow_data.csv", FILE_READ);
    if (!file) {
        server.send(404, "text/plain", "File not found");
        return;
    }
    server.streamFile(file, "text/csv");
    file.close();
}
