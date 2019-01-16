#include <Servo.h>

#include <IRremote.h>
/**
 * Simple timer alarm that turns servo to turn on light
 * basic functionality requires a screen (try sev seg?), remote, servo motor
 */

enum remote_cmd {
  none,
  number,     //entering number value
  enter,      //submitting value (new timer number etc.)
  cancel      //cancel current buffered value or press twice for removing timer?
};

const int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);
decode_results results;

Servo servo1;

static remote_cmd state = none;
static int angle = 0;
int change;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
  //setup remote control
  pinMode(1, OUTPUT);
  servo1.attach(14);
}

void loop() {
  // put your main code here, to run repeatedly:
  //while time is > 0, cycle through timer and count down, checking for remote input
  if (irrecv.decode(&results)){
    Serial.println(results.value, HEX);
    switch (results.decode_type){
      
    }
    irrecv.resume();
  }

  if (angle >= 180){
    change = -1;
  }else if (angle <= 0){
    change = 1;
  }
  servo1.write(angle + change);
  
}


