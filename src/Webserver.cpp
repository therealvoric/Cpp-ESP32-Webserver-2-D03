
/*
 * License: GNU GPL v3.0
 *
 * Created by therealvoric on GitHub
 * 🚀 on 6.12.23, 7:48 AM
 * Version 2.12.2
 * Explore my contributions at: https://github.com/therealvoric
 */

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#define LED1_PIN 14
#define LED2_PIN 27
#define LED3_PIN 32
#define LED4_PIN 33
#define BTNS2 25
#define BTNS3 13
#define BTNS4 15

const char* ssid = "ESP32_YilD";
const char* password = "11223344";

IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

WebServer server(80);

bool LED1status = LOW;
bool blinkLED1 = false;
unsigned long previousMillis_LED1 = 0;
const long interval_LED1 = 500;

bool LED2status = LOW;
bool LED3status = LOW;
int LED4brightness = 0;

bool btns2state = false;
bool btns3state = false;
bool btns4state = false;





String SendHTML(uint8_t led1stat, uint8_t led2stat, uint8_t led3stat, int led4brightness, bool btns2, bool btns3, bool btns4) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>LED Control</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px; background-color: #f2f2f2;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: inline-block;width: 120px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 16px;margin: 10px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #3498db;}\n";
  ptr += ".button-on:active {background-color: #2980b9;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "input[type=range] {width: 80%;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";

  ptr += "<h1>ESP33 Schnittstelle </h1>\n";
  ptr += "<h3>Der ESP32 wird momentan als AP (ACCESS POINT) verwendet</h3>\n";

  if (led1stat) {
    ptr += "<p>LED1 Status: ON</p><button class=\"button button-off\" onclick=\"window.location.href='/led1off'\">OFF</button>\n";
  } else {
    ptr += "<p>LED1 Status: OFF</p><button class=\"button button-on\" onclick=\"window.location.href='/led1on'\">ON</button>\n";
    ptr += "<button class=\"button button-on\" onclick=\"window.location.href='/blinkled1'\">Blink LED1</button>\n";
  }

  if (led2stat) {
    ptr += "<p>LED2 Status: ON</p><button class=\"button button-off\" onclick=\"window.location.href='/led2off'\">OFF</button>\n";
  } else {
    ptr += "<p>LED2 Status: OFF</p><button class=\"button button-on\" onclick=\"window.location.href='/led2on'\">ON</button>\n";
  }

  if (led3stat) {
    ptr += "<p>LED3 Status: ON</p><button class=\"button button-off\" onclick=\"window.location.href='/led3off'\">OFF</button>\n";
  } else {
    ptr += "<p>LED3 Status: OFF</p><button class=\"button button-on\" onclick=\"window.location.href='/led3on'\">ON</button>\n";
  }

  ptr += "<p>Helligkeit von LED4: <input type=\"range\" id=\"brightnessSlider\" min=\"0\" max=\"255\" value=\"" + String(led4brightness) + "\"> <span id=\"brightnessValue\">Wert: " + String(led4brightness) + "</span></p>\n";

  ptr += "<p id=\"btnsState\">BTNS2: " + String(btns2 ? "<span style='color:green;'>Pressed</span>" : "Not Pressed") + " | BTNS3: " + String(btns3 ? "<span style='color:green;'>Pressed</span>" : "Not Pressed") + " | BTNS4: " + String(btns4 ? "<span style='color:green;'>Pressed</span>" : "Not Pressed") + "</p>\n";

  ptr += "</body>\n";
  ptr += "<script>\n";
  ptr += "function updateBrightness() {\n";
  ptr += "   var brightnessValue = document.getElementById('brightnessSlider').value;\n";
  ptr += "   document.getElementById('brightnessValue').innerText = 'Wert: ' + brightnessValue;\n";
  ptr += "   fetch('/applybrightness?value=' + brightnessValue);\n";
  ptr += "}\n";
  ptr += "function queryButtonStates() {\n";
  ptr += "   fetch('/buttonstates')\n";
  ptr += "      .then(response => response.text())\n";
  ptr += "      .then(data => document.getElementById('btnsState').innerHTML = data);\n";
  ptr += "}\n";
  ptr += "document.getElementById('brightnessSlider').addEventListener('input', updateBrightness);\n";
  ptr += "setInterval(queryButtonStates, 100); // Update button states every second\n";
  ptr += "</script>\n";
  ptr += "</html>\n";
  return ptr;
}





void handle_OnConnect() {
  LED1status = LOW;
  LED2status = LOW;
  LED3status = LOW;
  Serial.println("GPIO27 Status: OFF | GPIO32 Status: OFF | GPIO33 Status: OFF | GPIO14 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4brightness, btns2state, btns3state, btns4state));
}

void handle_blinkled1() {
  blinkLED1 = !blinkLED1;
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4brightness, btns2state, btns3state, btns4state));
}

void handle_applybrightness() {
  String brightnessValue = server.arg("value");
  LED4brightness = brightnessValue.toInt();
  analogWrite(LED4_PIN, LED4brightness);
  server.send(200, "text/html", SendHTML(LED1status, LED2status, LED3status, LED4brightness, btns2state, btns3state, btns4state));
}

void handle_led1on() {
  LED1status = HIGH;
  Serial.println("LED1 Status: ON");
  server.send(200, "text/html", SendHTML(true, LED2status, LED3status, LED4brightness, btns2state, btns3state, btns4state));
}

void handle_led1off() {
  LED1status = LOW;
  Serial.println("LED1 Status: OFF");
  server.send(200, "text/html", SendHTML(false, LED2status, LED3status, LED4brightness, btns2state, btns3state, btns4state));
}

void handle_led2on() {
  LED2status = HIGH;
  Serial.println("LED2 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status, true, LED3status, LED4brightness, btns2state, btns3state, btns4state));
}

void handle_led2off() {
  LED2status = LOW;
  Serial.println("LED2 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, false, LED3status, LED4brightness, btns2state, btns3state, btns4state));
}

void handle_led3on() {
  LED3status = HIGH;
  Serial.println("LED3 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, true, LED4brightness, btns2state, btns3state, btns4state));
}

void handle_led3off() {
  LED3status = LOW;
  Serial.println("LED3 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, LED2status, false, LED4brightness, btns2state, btns3state, btns4state));
}

void handle_buttonstates() {
  server.send(200, "text/plain", "BTNS2: " + String(btns2state) + " | BTNS3: " + String(btns3state) + " | BTNS4: " + String(btns4state));
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);
  pinMode(BTNS2, INPUT_PULLUP);
  pinMode(BTNS3, INPUT_PULLUP);
  pinMode(BTNS4, INPUT_PULLUP);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  server.on("/", HTTP_GET, handle_OnConnect);
  server.on("/led1on", HTTP_GET, handle_led1on);
  server.on("/led1off", HTTP_GET, handle_led1off);
  server.on("/led2on", HTTP_GET, handle_led2on);
  server.on("/led2off", HTTP_GET, handle_led2off);
  server.on("/led3on", HTTP_GET, handle_led3on);
  server.on("/led3off", HTTP_GET, handle_led3off);
  server.on("/blinkled1", HTTP_GET, handle_blinkled1);
  server.on("/applybrightness", HTTP_GET, handle_applybrightness);
  server.on("/buttonstates", HTTP_GET, handle_buttonstates);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  if (blinkLED1) {
    unsigned long currentMillis_LED1 = millis();
    if (currentMillis_LED1 - previousMillis_LED1 >= interval_LED1) {
      previousMillis_LED1 = currentMillis_LED1;
      LED1status = !LED1status;
      digitalWrite(LED1_PIN, LED1status ? HIGH : LOW);
    }
  } else {
    if (LED1status) {
      digitalWrite(LED1_PIN, HIGH);
    } else {
      digitalWrite(LED1_PIN, LOW);
    }
  }

  if (LED2status) {
    digitalWrite(LED2_PIN, HIGH);
  } else {
    digitalWrite(LED2_PIN, LOW);
  }

  if (LED3status) {
    digitalWrite(LED3_PIN, HIGH);
  } else {
    digitalWrite(LED3_PIN, LOW);
  }

  btns2state = digitalRead(BTNS2) == LOW;
  btns3state = digitalRead(BTNS3) == LOW;
  btns4state = digitalRead(BTNS4) == LOW;
}
