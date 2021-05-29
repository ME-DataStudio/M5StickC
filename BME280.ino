/************************************************************************

  M5StickC I2C BME280 Sensor
  
  Reads Temperature, Humidity and Pressure from BME280.
  
  Uses Farmer Keith BMP280/BME280 library:
    https://github.com/farmerkeith/BMP280-library
  
  28-05-2021: Added wifi.
  29-05-2021: Added MQTT. 
************************************************************************/
#include <M5StickC.h>
#include <farmerkeith_BMP280.h>
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>

//BME sensor
BME280 bme;

//Wifi constants and variables
const char* ssid     = "your_network_name";
const char* password = "your_network_password";
const int httpPort = 80;
IPAddress local_IP(192, 168, 1, 210);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);
WiFiClient client;

//MQTT
const char* mqtt_server = "YOUR_MQTT_BROKER_IP_ADDRESS";
const int mqttPort = 1883;
long lastMsg = 0;
PubSubClient MQTTclient(client);

void setup() 
{
  //M5StickC init and TFT
  M5.begin(true,true,true);
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen( BLACK );
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextColor(YELLOW);  
  M5.Lcd.setTextSize(2);
  M5.Lcd.fillScreen( BLACK );
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("M5StickC BMP280");

  //Serial monitor
  Serial.begin(115200);
  Serial.println("M5StickC BMP280");
  
  //WIFI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("ESP Mac Address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway IP: ");
  Serial.println(WiFi.gatewayIP());
  
  // Use WiFiClient class to create connection to MQTT broker
  MQTTclient.setServer(mqtt_server, mqttPort);
  //MQTTclient.setCallback(callback);
  
  //I2C ans BME sensor
  Wire.begin();
  bme.begin();

  delay(3000);
  M5.Lcd.fillScreen( BLACK );
}

int textColor=YELLOW;

void reconnect() {
  // Loop until we're reconnected
  while (!MQTTclient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (MQTTclient.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      //MQTTclient.subscribe("esp32/output");
    } else {
      Serial.print("failed, rc=");
      Serial.print(MQTTclient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//MAIN loop
void loop() 
{
  if (!MQTTclient.connected()) {
    reconnect();
  }
  MQTTclient.loop();
  
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
  
    //BME variables declaration, assignment and conversion
    double temp;
    double druk;
    double vocht=bme.readHumidity(temp,druk);
    char tempString[8];
    char drukString[8];
    char vochtString[8];
    dtostrf(temp, 1, 2, tempString);
    dtostrf(druk, 1, 2, drukString);
    dtostrf(vocht, 1, 2, vochtString);
  
    //TFT print
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.print("Temp=");
    M5.Lcd.println(temp,2);
    M5.Lcd.print("Vocht=");
    M5.Lcd.println(vocht,2);
    M5.Lcd.print("Druk=");
    M5.Lcd.println(druk,2);

    //Serial monitor
    Serial.print("Temperatuur: ");
    Serial.println(tempString);
    Serial.print("Luchtdruk: ");
    Serial.println(drukString);
    Serial.print("Vocht: ");
    Serial.println(vochtString);
  
    // Send over with MQTT  
    MQTTclient.publish("esp32/temperatuur", tempString);
    MQTTclient.publish("esp32/druk", drukString);
    MQTTclient.publish("esp32/vocht", vochtString);
    
    if(textColor==YELLOW) textColor=GREEN;
    else textColor=YELLOW;
    M5.Lcd.setTextColor(textColor,BLACK);  
  }
}
