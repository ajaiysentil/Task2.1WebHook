#include <WiFiNINA.h>
#include "secrets.h"
#include "ThingSpeak.h"
#include <DHT.h>

#define DHTPIN 7      // Pin connected to the DHT sensor
#define DHTTYPE DHT11 // Type of DHT sensor

#define SECRET_SSID 
#define SECRET_PASS 
#define SECRET_CH_ID    
#define SECRET_WRITE_APIKEY 

DHT dht(DHTPIN, DHTTYPE);


char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int keyIndex = 0;
WiFiClient client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char *myWriteAPIKey = SECRET_WRITE_APIKEY;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ;
  }

  ThingSpeak.begin(client);

  dht.begin();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  ThingSpeak.setField(1, temperature); // Assuming field 1 is for temperature
  ThingSpeak.setField(2, humidity);    // Assuming field 2 is for humidity

  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  delay(20000); // Wait 20 seconds to update the channel again
}

