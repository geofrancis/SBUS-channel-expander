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
int THR = 0;


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

void setup()
{

  Serial.begin(115200);
  Serial.println("START");
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
  smthrottle.writeMicroseconds(PWMMid);
  sothrottle.writeMicroseconds(PWMMid);

  Serial.println("SETUP");

}


void loop() {

  if (sbus.waitFrame()) {

    FUNCTION = sbus.getChannel(7);

  }

  if (FUNCTION <= 1000) {
    fwinch.writeMicroseconds(PWMMid);
    twinch.writeMicroseconds(PWMMid);
  }
  if (FUNCTION > 1001 && FUNCTION < 1049) {
    
    Serial.println("SMOKE OFF");
    smoke.writeMicroseconds(PWMLow);
    smthrottle.writeMicroseconds(PWMLow);
   }
  if (FUNCTION > 1051 && FUNCTION < 1100) {
    Serial.println("SMOKE FULL");
    smoke.writeMicroseconds(PWMHigh);
    smthrottle.writeMicroseconds(PWMHigh);
    THR = 0;
  }
  if (FUNCTION > 1101 && FUNCTION < 1149) {
    THR = 1;
    Serial.println("SMOKE THR");
    smoke.writeMicroseconds(PWMHigh);
   
  }
  if (FUNCTION > 1151 && FUNCTION < 1200) {
    sound.writeMicroseconds(PWMLow);
    Serial.println("SOUND OFF");
  }
  if (FUNCTION > 1201 && FUNCTION < 1249) {
    sound.writeMicroseconds(PWMHigh);
    Serial.println("SOUND IDLE");
  }
  if (FUNCTION > 1251 && FUNCTION < 1300) {
    sound.writeMicroseconds(PWMHigh);
    Serial.println("SOUND THR");
  }
  if (FUNCTION > 1301 && FUNCTION < 1349) {
    lights.writeMicroseconds(PWMLow);
    Serial.println("Lights OFF");
  }
  if (FUNCTION > 1351 && FUNCTION < 1400) {
    lights.writeMicroseconds(PWMHigh);
    Serial.println("Lights ON");
  }
  if (FUNCTION > 1401 && FUNCTION < 1449) {
    fwinch.writeMicroseconds(PWMHigh);
    Serial.println("Anchor Winch UP");
  }
  if (FUNCTION > 1451 && FUNCTION < 1500) {
    fwinch.writeMicroseconds(PWMLow);
    Serial.println("Anchor Winch down");
  }
  if (FUNCTION > 1501 && FUNCTION < 1549) {
    twinch.writeMicroseconds(PWMHigh);
    Serial.println("Tow Winch UP");
  }
  if (FUNCTION > 1551 && FUNCTION < 1600) {
    twinch.writeMicroseconds(PWMLow);
    Serial.println("Tow Winch down");
  }
  else if (FUNCTION >= 2000) {
    functionselect = 11;
  }


  //sothrottle.writeMicroseconds( sbus.getChannel(3) );
  if     (THR == 1) {
    smthrottle.writeMicroseconds(sbus.getChannel(3) );
  }
  if     (THR == 0) {
    smthrottle.writeMicroseconds(PWMLow);
  }


  crane.writeMicroseconds( sbus.getChannel(15) );
  Serial.println(sbus.getChannel(7));
  Serial.println(THR);


  delay(100);
}
