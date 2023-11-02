#include <sbus.h>
#include <Servo.h>


/*functions

   1, smoke power off/on/onmax
   2, sound power on/off
   3, lights      on off
   4, front winch up/off/down
   5, tow winch   up/off/down
   6, throttle    analogue
   7, crane       analogue
*/
// used pins
#define SBUS_PIN 3   // D3

#define smokepin   A0   
#define soundpin   A1  
#define lightspin  A2   
#define fwinchpin  A3  
#define twinchpin  A4   
#define smthrottlepin A5   
#define sothrottlepin A6   
#define cranepin A7   

#define PWMHigh 2000
#define PWMMid  1500
#define PWMLow  1000



int FUNCTION = 0 ;
int functionselect = 0;



SBUS sbus;

Servo smoke;
Servo sound;
Servo lights;
Servo fwinch;
Servo twinch;

Servo smthrottle;
Servo sothrottle;
Servo crane;
int smthrottlepwm;
int sothrottlepwm;
int cranepwm;

void setup() {
  sbus.begin(SBUS_PIN, sbusBlocking);
  
  smoke.attach(smokepin); 
  sound.attach(soundpin); 
  lights.attach(lightspin); 
  fwinch.attach(fwinchpin); 
  twinch.attach(twinchpin); 
  smthrottle.attach(smthrottlepin); 
  sothrottle.attach(sothrottlepin); 
  crane.attach(cranepin); 



  
  //set startup positions
  smoke.writeMicroseconds(PWMLow);
  sound.writeMicroseconds(PWMLow);
  lights.writeMicroseconds(PWMLow);
  
  fwinch.writeMicroseconds(PWMMid);
  twinch.writeMicroseconds(PWMMid);
  
  crane.writeMicroseconds(PWMMid);
  throttle.writeMicroseconds(PWMMid);
}


void loop() {

  if (sbus.waitFrame()) {

    FUNCTION = sbus.getChannel(16);

    if (FUNCTION <= 1000) {
      fwinch.writeMicroseconds(PWMMid);
      twinch.writeMicroseconds(PWMMid);
      smoke.writeMicroseconds(PWMLow);
    }
    if (FUNCTION > 1001 && FUNCTION < 1049) {
      smoke.writeMicroseconds(PWMLow);
    }
    if (FUNCTION > 1051 && FUNCTION < 1100) {
      smoke.writeMicroseconds(PWMHigh);
    }
    if (FUNCTION > 1101 && FUNCTION < 1149) {
      sound.writeMicroseconds(PWMLow);
    }
    if (FUNCTION > 1151 && FUNCTION < 1200) {
      sound.writeMicroseconds(PWMHigh);
    }
    if (FUNCTION > 1201 && FUNCTION < 1249) {
      lights.writeMicroseconds(PWMLow);
    }
    if (FUNCTION > 1251 && FUNCTION < 1300) {
      lights.writeMicroseconds(PWMHigh);
    }
    if (FUNCTION > 1301 && FUNCTION < 1349) {
      fwinch.writeMicroseconds(PWMLow);
    }
    if (FUNCTION > 1351 && FUNCTION < 1400) {
      fwinch.writeMicroseconds(PWMHigh);
    }
    if (FUNCTION > 1401 && FUNCTION < 1449) {
      twinch.writeMicroseconds(PWMHigh);
    }
    if (FUNCTION > 1901 && FUNCTION < 2000) {
      twinch.writeMicroseconds(PWMLow);
    }
    else if (FUNCTION >= 2000) {
      functionselect = 11;
    }

    throttle.writeMicroseconds( sbus.getChannel(14) );
    crane.writeMicroseconds( sbus.getChannel(15) );

  }

  delay(20);

}
