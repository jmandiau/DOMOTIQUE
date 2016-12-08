#include <U8glib.h>
#include <DHT.h>
 
#define DHTPIN 4     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

//Obj DHT
DHT dht(DHTPIN, DHTTYPE);
//Obj Oled
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);

//Constantes 
//temps entre chaque lecture
const int readTime(2000);
//Decalage scroll
const int v_inc = 2;
int v_diff=0;
int v_diffTime=millis();

//Variables
//Init position scroll
int v_xoffset = 0;
//compteur
float tmpHumidity=0;
float tmpTemperature=0;

//Function draw 
void draw(String message,float humidity,float temperature) {
  // graphic commands to redraw the complete screen should be placed here  

  u8g.firstPage();  
  do {
    u8g.setPrintPos(v_xoffset, 15);
    u8g.print(message);

    u8g.setPrintPos(0, 30);
    u8g.print("Hum: "+String(humidity,2)+"%");

    u8g.setPrintPos(0, 45);
    u8g.print("Temp:"+String(temperature,2)+"C");
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

void setup() {
  
  Serial.begin(9600); 
  //while (!Serial) {
    //; // wait for serial port to connect. Needed for native USB
  //}
  
  //Init sensor
  dht.begin();
  
  // flip screen, if required
  u8g.setRot180();
  //Define font
  u8g.setFont(u8g_font_unifont);
  //u8g.sleepOn();
  //u8g.sleepOff();


}

void loop() {


  v_diff=millis()-v_diffTime;
  
  if(v_diff>=readTime){
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
  draw("Welcome Joffrey & Maryne",tmpHumidity,tmpTemperature);
  delay(50); 
}
