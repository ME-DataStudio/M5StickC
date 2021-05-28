/************************************************************************

  M5StickC I2C BME280 Sensor
  
  Reads Temperature, Humidity and Pressure from BME280.
  
  Uses Farmer Keith BMP280/BME280 library:
    https://github.com/farmerkeith/BMP280-library

************************************************************************/
#include <M5StickC.h>
#include <farmerkeith_BMP280.h>
#include <Wire.h>

//BMP280 bmp0;
BME280 bme;
float tmp = 0.0;
float hum = 0.0;
float pressure = 0.0;

void setup() 
{
  M5.begin(true,true,true);
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen( BLACK );
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextColor(YELLOW);  
  M5.Lcd.setTextSize(2);

  M5.Lcd.fillScreen( BLACK );
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.println("M5StickC BMP280");

  Serial.begin(115200);
  Serial.println("M5StickC BMP280");
    
  Wire.begin();
  //bmp0.begin();
  bme.begin();

  delay(3000);
  M5.Lcd.fillScreen( BLACK );
}

int textColor=YELLOW;

void loop() 
{
  int address;
  int error;
  //double temperature=bmp0.readTemperature ();
  double temp;
  double druk;
  double vocht=bme.readHumidity(temp,druk);
  
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print("Temp=");
  M5.Lcd.println(temp,2);
  M5.Lcd.print("Vocht=");
  M5.Lcd.println(vocht,2);
  M5.Lcd.print("Druk=");
  M5.Lcd.println(druk,2);

  
  Serial.printf("Temperatuur: %2.2f*C\n\r"  , temp);
  Serial.printf("Vocht: %2.2f\%\n\r", vocht);
  Serial.printf("Druk: %2.2fhPa\n\r", druk);
  
  delay(1000);
  
  if(textColor==YELLOW) textColor=GREEN;
  else textColor=YELLOW;
  M5.Lcd.setTextColor(textColor,BLACK);  
}
