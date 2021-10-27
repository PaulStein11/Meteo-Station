#include <RTClib.h>

#include <Adafruit_Sensor.h>

#include <SPI.h>
#include <SD.h>
#include <RTClib.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#define DHTTYPE DHT11
#define DHTPIN 7
#define Up 3
#define Down 5
#define Select 4
//#define pinCS 4

//Declaration of sensors from library
RTC_DS1307 RTC;
LiquidCrystal_I2C lcd(0x3f, 16, 2);
DHT dht (DHTPIN,DHTTYPE);
File file;

long id = 1;
int menu = 1;
char timedate[32];

//Values of floor moisture
int sensorPin = A0;  
int sensorValue = 0; 
int percent = 0;
String percentString = "0";
int stringLength = 0;


//Characters from menu
byte dot[8] = {B00100,B00100,B01010,B01010,B10001,B10001,B10001,B01110};
byte termo[8] = {B01110,B01010,B01010,B01010,B01110,B11111,B11111,B01110};
byte degree[8] = {B00110,B01001,B01001,B00110,B00000,B00000,B00000,B00000};


//Percentage of moistured floor

void setup() {
  Serial.begin(9600);
  pinMode(Up, INPUT_PULLUP);
  pinMode(Down, INPUT_PULLUP);
  pinMode(Select, INPUT_PULLUP);
  //pinMode(pinCS, OUTPUT);
  dht.begin();
  Wire.begin();
  RTC.begin();
  //RTC.adjust(DateTime(__DATE__, __TIME__)); // Establece la fecha y hora (Comentar una vez establecida la hora)
  //el anterior se usa solo en la configuracion inicial
  
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("METEOSTATION");
  delay(1000);
  
  lcd.createChar(3,degree);
  lcd.createChar(2,termo);
  lcd.createChar(1,dot);

  mainMenu();
  
 // if(!SD.begin(pinCS)){
  Serial.println("SD was not initialize");
}
//}

void loop() {
  //MENU //Repetition of the functions created
  Serial.println(11);
  if (!digitalRead(Down)){
    menu ++;
    mainMenu();
    delay(100);
    while(!digitalRead(Down));
     }

  if (!digitalRead(Up)){
    menu --;
    mainMenu();
    delay(100);
    while(!digitalRead(Up));
     }

  if (!digitalRead(Select)){
    menuaction();
    mainMenu();
    while(!digitalRead(Select));
    }
}

void mainMenu(){
  switch(menu){
    
    case 0:
    menu = 1;
    break;
    
    case 1:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("> 1. WEATHER");
    lcd.setCursor(0,1);
    lcd.print("  2. DATE/ TIME");
    break;

    case 2:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  1. WEATHER");
    lcd.setCursor(0,1);
    lcd.print("> 2. DATE/ TIME");
    break;
    
    case 3:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("> 3. FLOOR MOISTURE");
    lcd.setCursor(0,1);
    lcd.print("  4. RECORD DATA");

    break;

    case 4:
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  3. FLOOR MOISTURE");
    lcd.setCursor(0,1);
    lcd.print("> 4. RECORD DATA");
    break;

    case 5:
    menu = 4;
    break;
  }
}

void menuaction(){
  
  switch(menu){
    
    case 1:
    action1();
    break;

    case 2:
    action2();
    break;

    case 3:
    action3();
    break;
      
    /*case 4:
    action4();
    break;*/
  }
}

void action1(){

   float h = dht.readHumidity();
   float t = dht.readTemperature();
   
  if(isnan(h) || isnan(t))
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("FAILED");
    return;
  }
  //HUMIDITY
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("HUMID: ");
  lcd.setCursor(7,0);
  lcd.write(char(1));
  lcd.setCursor(9,0);
  lcd.print(h);
  lcd.print("%");

  //TEMPERATURE
  lcd.setCursor(0,1);
  lcd.print("TEMPT:  ");
  lcd.setCursor(7, 1);
  lcd.write(char(2));
  lcd.setCursor(9,1);
  lcd.print(t);
  lcd.write(char(3));
  lcd.print("C ");
  delay(4000);
}

void action2(){
    
for(int count =0; count <=3; count++){
  
  lcd.clear();
  lcd.setCursor(0,0);
  DateTime now = RTC.now();
  lcd.print("Date:");
  lcd.print(now.year(), DEC);
  lcd.print("/");
  lcd.print(now.month(), DEC);
  lcd.print("/");
  lcd.print(now.day(), DEC);
  lcd.print(" ");
  lcd.setCursor(0,1);
  lcd.print("Time: ");
  lcd.print(now.hour(), DEC);
  lcd.print(":");
  lcd.print(now.minute(), DEC);
  lcd.print(":");
  lcd.print(now.second(), DEC);
  delay(1000); // La informaci¢n se actualiza cada 1 seg.
  lcd.setCursor(0,0);
  lcd.print("               ");
  lcd.setCursor(0,1);
  lcd.print("               ");
 

  }
}

void action3(){

  sensorValue = analogRead(sensorPin);
  percent = convertToPercent(sensorValue);
  percentString = String(percent);
  stringLength = percentString.length();

  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("FLOOR MOISTURE");
  delay(1500);

  lcd.setCursor(6,1);

  lcd.print(stringLength);
  lcd.print("%");
  delay(3000);
  
}
  int convertToPercent( int value) {
    int percentValue = 0;
    percentValue = map(value, 1023, 465, 0, 100);
    if(percentValue>100) {
      percentValue = 100;
      return percentValue;
    }
  }

/*void action4(){
  
DateTime now = RTC.now();
  file = SD.open("TEST.txt", FILE_WRITE);
  if(file){
    String header = "ID | Tempt | Humid | Moist | Time";
    file.println(header);
    file.close(); 
    
  }

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  sensorValue = analogRead(sensorPin);
  percent = convertToPercent(sensorValue);
 
  String dataString = String(id) + "  | " + String(t) + " | " + String(h) + " | " + String(percent) + "    | " + String (now.day()) + "/" + String(now.month())+ "/" +String(now.year()) + " " + String(now.hour())+ ":" + String(now.minute());
  delay(500);
  file = SD.open("TEST.txt", FILE_WRITE);
    if(file){
      file.println(dataString);
      delay(500);
      file.close();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Done");
      delay(800);
    
   }
    else{
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Cloudnt open file");
   }

    
}*/
