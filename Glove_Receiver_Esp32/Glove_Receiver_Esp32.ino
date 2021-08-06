
/* -- Wiredd --
  - D21 = SDA | D22 = SCL
  - D23 = MOSI
  - D18 = SCK
  - D19 = MISO
  - D5  = CS
  - VCC = 5V
*/
// -------------------------------------------------
const char* days[] = {"Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
const char* months[] = {"Januari", "Februari", "Maret", "April", "Mei", "Juni", "Juli", "Augustus", "September", "October", "November", "Desember"};
String AMPM;
byte second, minute, hour, weekday, monthday, month, year = 0;
const int setMenit = 14;
const int setJam = 22;
const int setHari = 1;
const int setTanggal = 26;
const int setBulan = 7;  // juli
const int setTahun = 21;  // 2021

#include <Wire.h>              // Wire library (required for I2C devices)
#include <Adafruit_BMP280.h>   // Adafruit BMP280 sensor library
#include <esp_now.h>
#include <WiFi.h>
#include <TinyGPS++.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
//#include <SoftwareSerial.h>
#include "HardwareSerial.h";


#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>
#include "oled.h"
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define SD_CS 5
#define alarm 2
#define BMP280_I2C_ADDRESS  0x76

const int I2C_ADDRESS = 0x68;
Adafruit_BMP280  bmp280;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

typedef struct struct_message {
  int id;
  int sinyal;
  int stats;
  double latitude_slave;
  double longitude_slave;
} struct_message;

struct_message myData;
struct_message board1;
struct_message board2;
struct_message boardsStruct[2] = {board1, board2};

// -------------------------------------------------
unsigned long previousMillis, previousMillis2, previousMillis3, previousMillis4 = 0;
bool bmp_cek, sdcard_cek, espnow_cek, oled_cek, gps_cek;
bool board1connected, board2connected, board3connected;
bool sekaliKirim = false;
bool check = true;
bool anj;
int temp;
int pressure;
float latitude, longitude;
String lat_str, lng_str;
String dataMessage, newMessage;
int counting = 60;
int lastValue1, lastValue2, lastValue3;
int lastVValue1;
int currentValue1, currentValue2, currentValue3;

TinyGPSPlus gps;

HardwareSerial SerialGPS(1);
//static const int RXPin = 16, TXPin = 17;
//SoftwareSerial ss(RXPin, TXPin); // The serial connection to the GPS device

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);

  Serial.println(macStr);

  memcpy(&myData, incomingData, sizeof(myData));

  Serial.printf("Board ID %u: %u bytes\n", myData.id, len);

  // Update the structures with the new incoming data
  boardsStruct[myData.id - 1].sinyal = myData.sinyal;
  boardsStruct[myData.id - 1].stats = myData.stats;
  boardsStruct[myData.id - 1].latitude_slave = myData.latitude_slave;
  boardsStruct[myData.id - 1].longitude_slave = myData.longitude_slave;
}


// config AP SSID
void configDeviceAP() {
  const char *SSID = "Slave";
  bool result = WiFi.softAP(SSID, "Slave_Password", 1, 0);
  if (!result) {
    Serial.println("AP Config failed.");
  } else {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
  }
}

void setup() {
  //Initialize Serial Monitor
  Serial.begin(115200);
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  pinMode(alarm, OUTPUT);
  configDeviceAP();

  (!bmp280.begin(BMP280_I2C_ADDRESS)) ? bmp_cek = false : bmp_cek = true;
  (!esp_now_init()) ? espnow_cek = false : espnow_cek = true;
  (!SD.begin(SD_CS)) ? sdcard_cek = false : sdcard_cek = true;
  (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) ? oled_cek = false : oled_cek = true;
//  mengaturWaktu();

  SerialGPS.begin(9600, SERIAL_8N1, 16, 17);
  everySDStart();
  esp_now_register_recv_cb(OnDataRecv);

}

void loop() {
  while (SerialGPS.available() > 0) {

    bacaGps();
    temp     = bmp280.readTemperature();
    pressure = bmp280.readPressure();
    membacaWaktu();
    // -- opening --
    display.clearDisplay();
    unsigned long currentMillis4 = millis();
    if (currentMillis4 - previousMillis4 >= 3000) { // ganti tiap 3 detik sekali tampilan nyo
      previousMillis4 = currentMillis4;
      if (anj == true) {
        digitalWrite(alarm, LOW);
        anj = false;
      } else {
        if (!board2connected || !board1connected) digitalWrite(alarm, HIGH);
        else digitalWrite(alarm, LOW);
        anj = true;
      }
    }
    statusbar();
    (anj) ? homescreen() : subscreen();
    storeDataToSD();

    checkStatus();
    checkConnected();
    boardNotConnected();
    // -- closing --
    display.display();
  }
}
