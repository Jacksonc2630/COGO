#include <SoftwareSerial.h>

#define RX 3
#define TX 2

char c = ' ';
boolean new_line = true;

SoftwareSerial BTSerial(RX, TX); // (RX, TX)

void setup() {  

  Serial.begin(9600);
 
  BTSerial.begin(9600);
 
  Serial.println("Enter AT commands:");
}


void loop() {
 
  if (BTSerial.available())
    Serial.write(BTSerial.read());

  if (Serial.available()) {

    c = Serial.read();

    if(c != 10 && c != 13)
      BTSerial.write(c);

    if (new_line) {
      Serial.print("\r\n>");
      new_line = false;
    }
    // Write to the Serial Monitor the bluetooth's response
    Serial.write(c);
   
    if (c == 10)
      new_line = true;
  }
}

