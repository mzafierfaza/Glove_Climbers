
#include <ESP8266WiFi.h>
#include <espnow.h>

#define BOARD_ID 1  // board master
// ESP32 Slave = 24:6F:28:9D:7D:0C
uint8_t broadcastAddress[] = {0x24, 0x6F, 0x28, 0x9D, 0x7D, 0x0C}; // MAC ADDRESS SLAVE

int sinyal;

typedef struct struct_message {
  int id;
  int sinyal;
  int stats;
} struct_message;

struct_message myData;

unsigned long lastTime = 0;
unsigned long timerDelay = 1000;

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("\r\nLast Packet Send Status: ");
  if (sendStatus == 0) {
    Serial.println("Delivery success");
  }
  else {
    Serial.println("Delivery fail");
  }
}


void ScanForSlave() {
  int8_t scanResults = WiFi.scanNetworks();
  bool slaveFound = 0;

  Serial.println("");
  if (scanResults == 0) {
    Serial.println("No WiFi devices in AP Mode found");
  } else {
    Serial.print("Found "); Serial.print(scanResults); Serial.println(" devices ");
    for (int i = 0; i < scanResults; ++i) {
      // Print SSID and RSSI for each device found
      String SSID = WiFi.SSID(i);
      int32_t RSSI = WiFi.RSSI(i);
      String BSSIDstr = WiFi.BSSIDstr(i);
      if (SSID.indexOf("Slave") == 0) {
        sinyal = RSSI;
        Serial.println("Found a Slave.");
        Serial.print(i + 1); Serial.print(": "); Serial.print(SSID); Serial.print(" ["); Serial.print(BSSIDstr); Serial.print("]"); Serial.print(" ("); Serial.print(RSSI); Serial.print(")"); Serial.println("");
        break;
      }
    }
  }
  
  WiFi.scanDelete();
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    ScanForSlave();
    myData.id = BOARD_ID;
    myData.sinyal = sinyal;
    myData.stats = random(1,100);
    
    esp_now_send(0, (uint8_t *) &myData, sizeof(myData));
    lastTime = millis();
  }
}
