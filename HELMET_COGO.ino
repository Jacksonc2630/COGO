int trigger_pin = 2;
int echo_pin = 3;
int buzzer_pin = 10;
int force_sensor_pin1 = A0;
int force_sensor_pin2 = A1;
int force_sensor_pin3 = A3;
int force1;
int force2;
int force3;
int time;


void setup ( ) {
       Serial.begin (9600);
       pinMode (trigger_pin, OUTPUT);
       pinMode (echo_pin, INPUT);
       pinMode (buzzer_pin, OUTPUT);
       pinMode (force_sensor_pin1, INPUT);
       pinMode (force_sensor_pin2, INPUT);
       pinMode (force_sensor_pin3, INPUT);
}
void loop ( ) {
   digitalWrite (trigger_pin, HIGH);
   delayMicroseconds (10);
   digitalWrite (trigger_pin, LOW);
   time = pulseIn (echo_pin, HIGH);
   force1 = analogRead(force_sensor_pin1);
   force2 = analogRead(force_sensor_pin2);
   force3 = analogRead(force_sensor_pin3);


if (force1>5)
       {     
       Serial.print (" force1= "); 
         if (force1<200) {
           Serial.print (" Light impact ");
         }
         if (force1>=200 and force1<400) {
           Serial.print (" medium impact ");
         }
         if (force1>=400) {
           Serial.print (" Big impact ");
         }
       digitalWrite (buzzer_pin, HIGH);
       delay (500);
       }
 else {
       Serial.print (" force1= ");
       Serial.println ("No Impact");     
       digitalWrite (buzzer_pin, LOW);
       delay (500);       
 }


if (force2>5)
       {     
       Serial.print (" force2= "); 
         if (force2<200) {
           Serial.print (" Light impact ");
         }
         if (force2>=200 and force2<400) {
           Serial.print (" medium impact ");
         }
         if (force2>=400) {
           Serial.print (" Big impact ");
         }
       digitalWrite (buzzer_pin, HIGH);
       delay (500);
       }
 else {
       Serial.print (" force2= ");
       Serial.println ("No Impact");     
       digitalWrite (buzzer_pin, LOW);
       delay (500);      
 }


if (force3>5)
       {   
       Serial.print (" force3= "); 
         if (force3<200) {
           Serial.print (" Light impact ");
         }
         if (force3>=200 and force3<400) {
           Serial.print (" medium impact ");
         }
         if (force3>=400) {
           Serial.print (" Big impact ");
         }
       digitalWrite (buzzer_pin, HIGH);
       delay (500);
       }
 else {
       Serial.print (" force3= ");
       Serial.println ("No Impact");     
       digitalWrite (buzzer_pin, LOW);
       delay (500);      
 }
}
