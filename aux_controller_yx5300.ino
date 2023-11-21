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
#define smthrottlepin 12
#define sothrottlepin 13
#define cranepin A7

#define PWMHigh 2000
#define PWMMid  1500
#define PWMLow  1000



int FUNCTION = 0 ;
int functionselect = 0;
int THRO = 0;
int THRM = 0;

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





static int8_t Send_buf[8] = {0} ;

#define CMD_PLAY_W_INDEX 0X03
#define CMD_SET_VOLUME 0X06
#define CMD_SEL_DEV 0X09
#define DEV_TF 0X02
#define CMD_PLAY 0X0D
#define CMD_PAUSE 0X0E
#define CMD_SINGLE_CYCLE 0X19
#define SINGLE_CYCLE_ON 0X00
#define SINGLE_CYCLE_OFF 0X01
#define CMD_PLAY_W_VOL 0X22

const int ledPin =  LED_BUILTIN;// the number of the LED pin
int ledState = LOW;
unsigned long previousMillis = 0;
const long interval = 1000;

void sendCommand(int8_t command, int16_t dat)
{
  delay(20);
  Send_buf[0] = 0x7e; //starting byte
  Send_buf[1] = 0xff; //version
  Send_buf[2] = 0x06; //the number of bytes of the command without starting byte and ending byte
  Send_buf[3] = command; //
  Send_buf[4] = 0x00;//0x00 = no feedback, 0x01 = feedback
  Send_buf[5] = (int8_t)(dat >> 8);//datah
  Send_buf[6] = (int8_t)(dat); //datal
  Send_buf[7] = 0xef; //ending byte
  for (uint8_t i = 0; i < 8; i++) //
  {
    Serial.write(Send_buf[i]) ;
  }
}


void setup()
{

  Serial.begin(9600);

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

  pinMode(ledPin, OUTPUT);

  Serial.println("SETUP");
  delay(2000);//Wait chip initialization is complete
  sendCommand(CMD_SEL_DEV, DEV_TF);//select the TF card
  delay(200);//wait for 200ms
  sendCommand(CMD_PLAY_W_VOL, 0X0F01);//play the first song with volume 15 class
}


void loop() {

  if (sbus.waitFrame()) {

    FUNCTION = sbus.getChannel(7);

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
      digitalWrite(ledPin, ledState);
    }
  }

  if (FUNCTION <= 1000) {
    fwinch.writeMicroseconds(PWMMid);
    twinch.writeMicroseconds(PWMMid);
   
  }
  if (FUNCTION > 1001 && FUNCTION < 1049) {

    Serial.println("SMOKE OFF");
    smoke.writeMicroseconds(PWMLow);
    THRM = 0;
    sendCommand(CMD_PLAY_W_VOL, 0X0F01);//play the first song with volume 15 class
  }
  if (FUNCTION > 1051 && FUNCTION < 1100) {
    THRM = 1;
    Serial.println("SMOKE FULL");
    smoke.writeMicroseconds(PWMHigh);
    
  }
  if (FUNCTION > 1101 && FUNCTION < 1149) {
    THRM = 2;
    Serial.println("SMOKE THRM");
    smoke.writeMicroseconds(PWMHigh);

  }
  if (FUNCTION > 1151 && FUNCTION < 1200) {
    sound.writeMicroseconds(PWMLow);
    Serial.println("SOUND OFF");
  }
  if (FUNCTION > 1201 && FUNCTION < 1249) {
    sound.writeMicroseconds(PWMHigh);
    Serial.println("SOUND IDLE");
    THRO = 0;
  }
  if (FUNCTION > 1251 && FUNCTION < 1300) {
    sound.writeMicroseconds(PWMHigh);
    Serial.println("SOUND THRM");
    THRO = 1;
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
  if (FUNCTION > 1601 && FUNCTION < 1650) {
    sendCommand(CMD_PLAY_W_VOL, 0x1E01);//play the first song with volume 15 class
    Serial.println("HORN 1");
  }
  if (FUNCTION > 1650 && FUNCTION < 1700) {
    sendCommand(CMD_PLAY_W_VOL, 0x1E02);//play the first song with volume 15 class
    Serial.println("HORN 2");
  }
  if (FUNCTION > 1700 && FUNCTION < 1750) {
    sendCommand(CMD_PLAY_W_VOL, 0x1E03);//play the first song with volume 15 class
    Serial.println("HORN 3");
  }
  if (FUNCTION > 1750 && FUNCTION < 1800) {
    sendCommand(CMD_PLAY_W_VOL, 0X0F01);//play the first song with volume 15 class
    Serial.println("HORN 4");
  }
  else if (FUNCTION >= 2000) {
    functionselect = 11;
  }




  if     (THRM == 0) {
    smthrottle.writeMicroseconds(PWMLow);
  }
  if     (THRM == 1) {
    smthrottle.writeMicroseconds(PWMHigh);
  }
  if     (THRM == 2) {
    smthrottle.writeMicroseconds(sbus.getChannel(16) );
  }


  if     (THRO == 0) {
    sothrottle.writeMicroseconds(PWMMid);
  }
  if     (THRO == 1) {
    sothrottle.writeMicroseconds(PWMHigh);
  }
  if     (THRO == 2) {
    sothrottle.writeMicroseconds(sbus.getChannel(16) );
  }




  crane.writeMicroseconds( sbus.getChannel(15) );
  Serial.println(sbus.getChannel(7));
  Serial.println(sbus.getChannel(16));
  Serial.println(THRM);


  delay(1);
}
