#include <Servo.h>
#include <IRremote.h>
/**
 * Simple timer alarm that turns servo to turn on light
 * basic functionality requires a screen (try sev seg?), remote, servo motor
 */

enum machine_state {
  transition_on,
  transition_off,
  on,
  off
};

const int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);
decode_results results;

Servo servo1;

static machine_state state = off;
long command_val;

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
    command_val = results.value;
    Serial.println(command_val, HEX);
    switch (command_val){
      case 0xFFA25D:
        //power: unsure of behaviour
        break;
      case 0xFFE01F:
        //down
        if (state == on){
          state = transition_off; 
        }
        break;
      case 0xFF906F:
        //up 
        if (state == off){
          state = transition_on;
        }
      default:
        //no input
        break;
    }
    irrecv.resume();
  }
  switch (state){
    case transition_on:
      if (angle < 180){
        servo1.write(angle += 1);
        delay(15);
      }else {
        state = on;
      }
      break;
    case transition_off:
      if (angle > 0){
        servo1.write(angle -= 1);
        delay(15);
      }else {
        state = off;
      }
      break;
    case on:
    case off:
    default:
      break;
  }
}


