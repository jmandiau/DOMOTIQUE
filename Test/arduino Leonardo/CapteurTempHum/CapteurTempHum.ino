#include <U8glib.h>
#include <DHT.h>
 
#define DHTPIN 4     // Pin du DHT
#define DHTTYPE DHT22   // DHT 22  (AM2302)

//Obj DHT
DHT dht(DHTPIN, DHTTYPE);
//Obj Oled
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

//Variables PIR
int inputPin = 7;
int readPIR = LOW; 

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
const int v_inc = 2;
//Init position scroll
int v_xoffset = 0;



//Function draw 
void draw(int draw) {
  // graphic commands to redraw the complete screen should be placed here  

  u8g.firstPage();  
  do {
     if(draw==1){
      homeMessage();
     }
     
     if(draw==2){
       informationMessage();
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
  u8g.drawStr( 0, 15, "Initialisation");
}

void informationMessage(){
  u8g.setFont(u8g_font_unifont);
  
  u8g.setPrintPos(v_xoffset, 15);
  u8g.print("Welcome Joffrey & Maryne");

  u8g.setPrintPos(0, 30);
  u8g.print("Hum: "+String(tmpHumidity,2)+"%");

  u8g.setPrintPos(0, 45);
  u8g.print("Temp:"+String(tmpTemperature,2)+"C");
  

}

void setup() {
  
  Serial1.begin(9600); 
  while (!Serial1) {
    ; // wait for serial port to connect. Needed for native USB
  }
  
  //Init sensor
  dht.begin();

  
  u8g.begin();
  // flip screen, if required
  u8g.setRot180();

  //Declare sensor PIR as input
  pinMode(inputPin, INPUT); 

  //Message d'accueil
  draw(1);
  delay(5000);

}

void loop() {

  if(v_diff>=readTimeDHT){
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
      //draw("Welcome Joffrey",tmpHumidity,tmpTemperature);
      //Serial.print("Humidity: "); 
      //Serial.print(humidity);
      //Serial.print(" %\t");
      //Serial.print("Temperature: "); 
      //Serial.print(temperature);
      //Serial.println(" *C");
    }
    v_diffTime=millis();
 
  }

  readPIR = digitalRead(inputPin);
  
  if (readPIR == LOW) {  //LOW=interruption par presence
      u8g.sleepOn();
  } else {
      u8g.sleepOff();
  }

  draw(2);

  v_diff=millis()-v_diffTime;
}
