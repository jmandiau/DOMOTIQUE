void setup() {
 //Initialize serial and wait for port to open:
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Hello world.\n");
  Serial.print("Coucou mon coeur <3.\n");
  delay(1000);
}
