#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>

Adafruit_BMP280 bmp;

float Temperatura;
float Presion, P0;

void setup()
{

Serial.begin(9600);
Serial.println("Iniciando:");
if( !bmp.begin()){
  Serial.println("BMP280 no encontrado !");
  while(1);
}

  P0 = bmp.readPressure()/100
}

void loop() {

  Temperatura = bmp.readTemperature();
  Presion = bmp.readPressure()/100;

  Serial.print("Temperatura: ");
  Serial.print(Temperatura);
  Serial.print(" C ");

  Serial.print("Presion: ");
  Serial.print(Presion);
  Serial.println(" hPa");
  
  Serial.print("Altitud aprox: ");
  Serial.print(bmp.readAltitude(P0));
  Serial.print(" m ");
  delay(2000);

}
