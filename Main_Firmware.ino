#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
ESP8266WebServer server(80);

// 📡 AP credentials
const char* apSSID = "ESP8266_Control";
const char* apPASS = "12345678";

// ---------- STATES ----------
enum Screen {
  BOOT,
  MAIN_MENU,
  SETTINGS,
  WIFI_SCAN,
  RADAR
};

Screen currentScreen = BOOT;

// -------- MENU --------
int menuIndex = 0;

String menuItems[] = {
  "Scaner",
  "Radar",
  "Settings"
};

const int menuLength = sizeof(menuItems) / sizeof(menuItems[0]);

// -------- WIFI SCAN --------
int networkCount = 0;


// ================= DISPLAY =================

void bootAnimation() {
  display.clearDisplay();
  for (int i = 0; i < 128; i += 4) {
    display.fillRect(0, 30, i, 5, SSD1306_WHITE);
    display.setCursor(20, 10);
    display.println("INITIALIZING");
    display.display();
    delay(40);
  }
  delay(400);
}

void drawStatusBar() {
  display.fillRect(0, 0, 128, 10, SSD1306_WHITE);
  display.setTextColor(SSD1306_BLACK);
  display.setCursor(2, 1);
  display.print("Zero Sniffer");
  display.setTextColor(SSD1306_WHITE);
}

void drawMenu() {
  display.clearDisplay();
  drawStatusBar();

  for (int i = 0; i < menuLength; i++) {
    if (i == menuIndex)
      display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
    else
      display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 14 + i * 12);
    display.println(menuItems[i]);
  }
  display.display();
}


// ================= WIFI SCANNER =================

void wifiScanner() {

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Scanning...");
  display.display();

  WiFi.mode(WIFI_AP_STA); // ensure scan allowed
  delay(100);

  networkCount = WiFi.scanNetworks();

  display.clearDisplay();
  drawStatusBar();

  if (networkCount == 0) {
    display.setCursor(0, 20);
    display.print("No networks");
  } else {
    display.setCursor(0, 14);
    display.print("Found:");
    display.print(networkCount);

    for (int i = 0; i < min(networkCount, 4); i++) {
      display.setCursor(0, 26 + i * 10);
      display.print(WiFi.SSID(i));
    }
  }

  display.display();
}


// ================= BUTTON SIMULATION =================

void pressUp() {
  menuIndex = (menuIndex - 1 + menuLength) % menuLength;
  drawMenu();
}

void pressDown() {
  menuIndex = (menuIndex + 1) % menuLength;
  drawMenu();
}

void pressOK() {

  if (menuIndex == 0) {
    wifiScanner();
  }
  else if (menuIndex == 1) {
    display.clearDisplay();
    display.setCursor(0, 25);
    display.print("Radar Active");
    display.display();
  }
  else if (menuIndex == 2) {
    display.clearDisplay();
    display.setCursor(0, 25);
    display.print("Settings");
    display.display();
  }
}


// ================= WEB SERVER =================

void handleRoot() {

  String page = R"rawliteral(
  <html>
  <head>
  <title>ESP Controller</title>
  <style>
  body { text-align:center; font-family:sans-serif; }
  button { width:140px; height:70px; font-size:22px; margin:12px; }
  </style>
  </head>
  <body>
  <h1>ESP8266 CONTROL</h1>
  <button onclick="fetch('/up')">UP</button><br>
  <button onclick="fetch('/ok')">OK</button><br>
  <button onclick="fetch('/down')">DOWN</button>
  </body>
  </html>
  )rawliteral";

  server.send(200, "text/html", page);
}

void setupServer() {

  server.on("/", handleRoot);

  server.on("/up", []() {
    pressUp();
    server.send(200, "text/plain", "UP");
  });

  server.on("/down", []() {
    pressDown();
    server.send(200, "text/plain", "DOWN");
  });

  server.on("/ok", []() {
    pressOK();
    server.send(200, "text/plain", "OK");
  });

  server.begin();
}


// ================= SETUP =================

void setup() {

  Wire.begin(D2, D1);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  bootAnimation();

  // 🚀 AP + STA MODE (CRITICAL)
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(apSSID, apPASS);

  display.clearDisplay();
  display.setCursor(0, 20);
  display.print("Connect to:");
  display.setCursor(0, 32);
  display.print(apSSID);
  display.setCursor(0, 44);
  display.print("192.168.4.1");
  display.display();

  setupServer();

  delay(2000);

  currentScreen = MAIN_MENU;
  drawMenu();
}


// ================= LOOP =================

void loop() {
  server.handleClient();
}
