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

    //set state from input command
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
        break;
      case 0xFF02FD:
        //play/pause - toggle servo
        if (state == off){
          state = transition_on;
        }else if (state == on) {
          state = transition_off;
        }
      default:
        //no input
        break;
    }
    irrecv.resume();
  }

  //determine action to take
  switch (state){
    case transition_on:
      servo1.write(180);
      delay(30);
      state = on;
      break;
    case transition_off:
      servo1.write(0);
      delay(30);
      servo1.write(90);
      state = off;
      break;
    case on:
    case off:
    default:
      break;
  }
}


