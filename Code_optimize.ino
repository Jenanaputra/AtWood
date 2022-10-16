#include <SoftwareSerial.h>

SoftwareSerial BTserial(11, 12); // RX | TX

/* Fix length of the track */
float Jtot = 0.5;
float Jcur = 0;
float velo = 0;

float acc = 0.0;

float tension = 0.0;

const float gravity = 9.8;  // m/s2
const float mb = 1.5;  // kg
const float ma = 2.5;  // kg
float friction = 0.0;
float friction_miu = 0.0; 



unsigned long time_, time_2, time_3, time_4, time_5, buffer_time, delta_time;
double time_1, time_2_, time_3_, time_4_, time_5_, buffer_time_;
bool finish, first_stop;


const int pingPin1 = 2; // Trigger Pin of Ultrasonic Sensor1
const int echoPin1 = 3; // Echo Pin of Ultrasonic Sensor1

const int pingPin2 = 9; // Trigger Pin of Ultrasonic Sensor2
const int echoPin2 = 10; // Echo Pin of Ultrasonic Sensor2

/* Initialize selenoid doorlocks */
int selenoid1 = 6;
int selenoid2 = 7;

/* Initialize Proximity pin */
const int sensor_pin = 5;

long duration1, inches1, duration2, inches2, cm2, prox_out;
double cm1, buffer_length, delta_distances;
int count, count_first;

float delta_time_float;

void setup() {
  // put your setup code here, to run once:
  time_ = 0.0;
  time_1 = 0.0;
  time_2= 0.0;
  finish = false;
  count = 0;
  buffer_length = 0;
  Serial.begin(9600);
  BTserial.begin(9600);
  
}

void measure_ultra1(){
   pinMode(pingPin1, OUTPUT);
   digitalWrite(pingPin1, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin1, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin1, LOW);
   pinMode(echoPin1, INPUT);
   duration1 = pulseIn(echoPin1, HIGH);
   cm1 = duration1 * 0.034 / 2;
}


void calculate_proximity(){
  prox_out = digitalRead(sensor_pin);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  measure_ultra1();
   time_ = millis();
   calculate_proximity();

   if (prox_out == 1) {
       measure_ultra1();
       if (finish != true){
          time_ = millis();
          delta_distances = abs(cm1 - buffer_length);
 
          if (delta_distances <= 0.5){
            count_first +=1;
            if (count_first == 1){
              first_stop = true;
              } else {
                first_stop = false;
                }

            if (first_stop == true){
              time_3 = millis() - time_2;
              time_4 = time_3;
              } else {
                  time_5 = millis() - time_2;
                  if (abs(time_5 - time_4) > 500 ){
                    buffer_time = time_5 - time_4;
                    finish = true;
                    } 
                      
                      
                }

          if (count_first == 10) 
              count_first = 2;
            } else{
              count_first = 0;
              }


          Serial.println("lanjut");
        } else {
          count +=1;
          if (count == 1){
            delta_time = time_ - time_2 - buffer_time;    
            delta_time_float = (float(delta_time))/1000.0;
            acc = (2*Jtot)/((delta_time_float)*(delta_time_float));
            velo = acc*delta_time_float;  // No.3
            tension = ((mb)*(gravity - acc)); //No.4
            friction = tension - (ma*acc); //No.5
            friction_miu = ((friction)/(ma*gravity)); //No.6

            
//            Serial.println(delta_time);
            Serial.println("stop");

            BTserial.print(delta_time_float);
            BTserial.print(",");
            BTserial.print(acc);
            BTserial.print(",");
            BTserial.print(velo);
            BTserial.print(",");
            BTserial.print(tension);
            BTserial.print(",");
            BTserial.print(friction);
            BTserial.print(",");
            BTserial.print(friction_miu);
            BTserial.print(";");   

            
            }
          }
          buffer_length = cm1;
          if (count == 10)
              count = 2;
    } else {
      Serial.println("wait");
      time_2 = millis();
      }
 


  
}
