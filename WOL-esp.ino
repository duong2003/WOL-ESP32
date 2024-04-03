#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Replace with your network credentials
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

// MAC address of the target computer
uint8_t targetMac[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55}; // Replace with your PC's MAC address

// Blynk auth token
char auth[] = "YourAuthToken";

BlynkTimer timer;

void sendWOL(uint8_t* mac) {
  // Build magic packet
  uint8_t packet[102];
  for (int i = 0; i < 6; i++) {
    packet[i] = 0xFF;
  }
  for (int i = 6; i < 102; i += 6) {
    memcpy(&packet[i], mac, 6);
  }

  // Send packet
  WiFiUDP udp;
  udp.beginPacket("255.255.255.255", 9); // Broadcast address and WOL port
  udp.write(packet, sizeof(packet));
  udp.endPacket();
}

void setup() {
  Serial.begin(115200);

  Blynk.begin(auth, ssid, password);

  timer.setInterval(1000L, checkButtonState); // Check button state every 1 second
}

void loop() {
  Blynk.run();
  timer.run();
}

void checkButtonState() {
  int buttonState = digitalRead(BUTTON_PIN); // Replace BUTTON_PIN with the pin connected to your Blynk button
  
  if (buttonState == HIGH) {
    // Button is pressed, send WOL packet
    sendWOL(targetMac);
    Serial.println("WOL packet sent");
    delay(1000); // Debounce delay
  }
}
