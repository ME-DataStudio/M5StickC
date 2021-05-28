/************************************************************************

  M5StickC I2C BME280 Sensor
  
  Reads Temperature, Humidity and Pressure from BME280.
  
  Uses Farmer Keith BMP280/BME280 library:
    https://github.com/farmerkeith/BMP280-library
  
  28-05-2021: Addes wifi.

************************************************************************/
#include <M5StickC.h>
#include <farmerkeith_BMP280.h>
#include <Wire.h>
#include <WiFi.h>

//BME sensor
BME280 bme;

//Wifi constants and variables
const char* ssid     = "your_network_name";
const char* password = "your_network_password";
IPAddress local_IP(192, 168, 1, 210);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 0, 0);
WiFiClient client;
const int httpPort = 80;
const int mqttPort = 1880;

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
  // Use WiFiClient class to create TCP connections
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  
  //I2C ans BME sensor
  Wire.begin();
  bme.begin();

  delay(3000);
  M5.Lcd.fillScreen( BLACK );
}

int textColor=YELLOW;

void loop() 
{
  //BME variables declaration and assignment
  double temp;
  double druk;
  double vocht=bme.readHumidity(temp,druk);
  
  //TFT print
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print("Temp=");
  M5.Lcd.println(temp,2);
  M5.Lcd.print("Vocht=");
  M5.Lcd.println(vocht,2);
  M5.Lcd.print("Druk=");
  M5.Lcd.println(druk,2);

  //Serial monitor
  Serial.printf("Temperatuur: %2.2f*C\n\r"  , temp);
  Serial.printf("Vocht: %2.2f\%\n\r", vocht);
  Serial.printf("Druk: %2.2fhPa\n\r", druk);
  
  delay(1000);
  
  if(textColor==YELLOW) textColor=GREEN;
  else textColor=YELLOW;
  M5.Lcd.setTextColor(textColor,BLACK);  
}
