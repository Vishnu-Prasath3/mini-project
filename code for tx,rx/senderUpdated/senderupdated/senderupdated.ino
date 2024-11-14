
#include <esp_now.h>
#include <WiFi.h>
#include <FastLED.h>

#define LED_PIN  16 // pin number 16 where connected
#define NUM_LEDS  16
CRGB leds[NUM_LEDS];
const int reedSwitchPin = 17;
const int capIn=18; //capacitive touch pin 
// [DEFAULT] ESP32 Board MAC Address: b4:8a:0a:8c:f9:e0
// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xb4, 0x8a, 0x0a, 0x8c, 0xf9, 0xe0}; //Match with Reciever Device MAC Address

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;
// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}


 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  pinMode(reedSwitchPin, INPUT);
  pinMode(capIn, INPUT);
  // FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, LED_PIN>(leds, NUM_LEDS);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }


}
 
void loop() {
  // Set values to send
  strcpy(myData.a, "esp ok");
  myData.b = random(1,20);
  myData.c = 1.2;
  myData.d = false;
  int sensorState = digitalRead(reedSwitchPin);

sensorState == LOW?myData.d=true:myData.d=false;
Serial.println(sensorState);
  delay(100); // Adjust the delay as needed

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(100);
 for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Red;
  }

  FastLED.show();

}