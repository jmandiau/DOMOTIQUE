#include <U8glib.h>
#include <DHT.h>
#include <RTClib.h>
#include <Wire.h>
 
#define DHTPIN 4     // Pin du DHT
#define DHTTYPE DHT22   // DHT 22  (AM2302)

//Obj DHT
DHT dht(DHTPIN, DHTTYPE);
//Obj Oled
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);
//obj Timer/clock
RTC_DS1307 cpClock; // Définition de l’objet Composant Timer Tiny RTC

//Variable clock
DateTime v_now;

//Variables PIR
int inputPin = 7;
int readPIR = 0;
int stateScreen=1;

//Variables DHT22
//temps entre chaque lecture
const unsigned long readTimeDHT(30000);
//V_diff > readTimeDHT : read first
unsigned long v_diff=9999999;
unsigned long v_diffTime=millis();
//Init capteur
float tmpHumidity=0;
float tmpTemperature=0;

//Variable Scroll
//Decalage scroll
const int v_inc = 4;
//Init position scroll
int v_xoffset = 0;


/*  
 *   Author:Seb
 *   Nom : FormatDate
Description : Formatage de la date
Paramètres : p_date => DateTime à formater
       p_type => 1 = date au format JJ/MM/AA, 2 = heure au format HH:MM:SS, 3 = date et heure au format JJ/MM/AA HH:MM:SS
Retour : String => Chaine contenant la date et l'heure.
*/
String FormatDate(DateTime p_date, int p_type)
{
  // Déclaration des variables 
  char v_timebuf[10];  // Time
  char v_datebuf[12];  // Date
  String v_return;
  // Formatage de la date et heure.
  sprintf(v_datebuf, "%02d/%02d/%02d", p_date.day(), p_date.month(), p_date.year());  // format date
  sprintf(v_timebuf, "%02d:%02d:%02d", p_date.hour(), p_date.minute(), p_date.second()); // format time

  switch (p_type)
  {
    case 1: 
    {
      v_return = String(v_datebuf);
      break;
    }
    case 2:
    {
      v_return = String(v_timebuf);
      break;
    }
    case 3:
    {
      v_return = String(v_datebuf) + " " + String(v_timebuf);
      break;
    }
  } 
  return v_return;
}

//Function setLcd 
void setLcd(char draw) {
  // graphic commands to redraw the complete screen should be placed here  

  u8g.firstPage();  
  do {   
      switch (draw)
      {
        case 'H': 
        {
          homeMessage();
          break;
        }
        case 'I':
        {
          informationMessage();
          break;
        }
      } 
     
  } while( u8g.nextPage() );
    //scroll gauche
  v_xoffset = v_xoffset - v_inc;
  if (v_xoffset <= -256){
    v_xoffset=0;
  }
  //Double scroll
  //if (v_xoffset <= -128 || v_xoffset >= 128){
    //v_inc = -v_inc;
  //}
}

void homeMessage(){
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr( 0, 15, "Init: OK");
  u8g.drawStr( 0, 30, "Turn on the");
  u8g.drawStr( 0, 45, "screen by motion");
  
}

void informationMessage(){
  u8g.setFont(u8g_font_unifont);
  
  u8g.setPrintPos(v_xoffset, 15);
  u8g.print("Welcome Joffrey & Maryne");

  u8g.setPrintPos(0, 30);
  u8g.print("Hum: "+String(tmpHumidity,2)+"%");

  u8g.setPrintPos(0, 45);
  u8g.print("Temp:"+String(tmpTemperature,2)+"C");

  u8g.setPrintPos(0, 60);
  u8g.print(FormatDate(v_now,3));
  

}

//Set les variables menbres tmpHumidity et tmpTemperature
void setTmpHum(){
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float humidity = dht.readHumidity();
    // Read temperature as Celsius
    float temperature = dht.readTemperature();
    
    // check if returns are valid, if they are NaN (not a number) then something went wrong!
    if (isnan(temperature) || isnan(humidity)) {
      //Serial.println("Failed to read from DHT");
    } else {
      tmpHumidity=humidity;
      tmpTemperature=temperature;
    }
}

//controle de l'ecran par motion sensor
void screenManagementByMotionSensor(){
  readPIR = digitalRead(inputPin);
  //Serial.println(readPIR);
  
  if (readPIR == HIGH) {  
    if(stateScreen == LOW){
     u8g.sleepOff();
     stateScreen=HIGH;
    }
   
  } else {
    if(stateScreen == HIGH){  
     u8g.sleepOn();
     stateScreen=LOW;
    }
  }
}

//Set la variable v_now
void setDateTime(){
  //Recuperation de la date/heure
  v_now = cpClock.now();
}


void setup() {
  
  Serial1.begin(9600); 
  while (!Serial1) {
    ; // wait for serial port to connect. Needed for native USB
  }
  
  //Init sensor temp/hum
  dht.begin();

  // Init du port I2C
  Wire.begin();
  
  u8g.begin();
  // flip screen, if required
  u8g.setRot180();

  //Declare sensor PIR as input
  pinMode(inputPin, INPUT); 

  // Init Tiny RTC pour obtenir l’horloge
  cpClock.begin();
   if (!cpClock.isrunning()) {
    //Serial.println("RTC is NOT running!");
    // Cette ligne permet de définir la date et l’heure du Tin RTC si celle ci n’a jamais été initialisé.
    // Attention à ne pas le faire fréquemment, en effet le nombre d’écriture est limité (espérance de vie du composant).
    cpClock.adjust(DateTime(__DATE__, __TIME__));
  }

  //Message d'accueil
  setLcd('H');
  delay(5000);

}

void loop() {

  setDateTime();
  
  if(v_diff>=readTimeDHT){
    setTmpHum();
    v_diffTime=millis();
 
  }
  
  screenManagementByMotionSensor();
  setLcd('I');
  v_diff=millis()-v_diffTime;
}
