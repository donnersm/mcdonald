/********************************************************************************************************************************************************

   Project:         Old Riddles die hard V3.0 / Old mac Donalds had a farm  / Riddle of farmer with Fox, chicken, sack of grain
   Target Platform: Arduino Nano                                                                                                                                                       *
   HardwareVersion: 3.0
   Firmware version 1.0
   Hardware setup: See github

   Mark Donners
   The Electronic Engineer
   Website:   www.theelectronicengineer.nl
   facebook:  https://www.facebook.com/TheelectronicEngineer
   youtube:   https://www.youtube.com/channel/UCm5wy-2RoXGjG2F9wpDFF3w
   github:    https://github.com/donnersm                                                                                                                                                       *
*********************************************************************************************************************************************************
  Version History
  1.0   first release

  Code build up
  1* #include libaries
  2* #define soundfiles
  3* #define buttons
  4* #define Items and item leds

  setup():
  5* initialize default positions of items
  6* Play welcome sound

  Main() loop:
  7* read switches
  8* respond to pressed switches

  functions:
  Move boat from side 1 to side 2
  Move boat from side 2 to side 1
  Move item from side 1 to side 2
  Mive item from side 2 to side 1
  Check for unwanted conditions
  Play victory sound and do a fictory dance

*/

#include <SD.h>                      // need to include the SD library
#include <TMRpcm.h>                  //  also need to include this library...
#include <ezButton.h>
#include <Adafruit_NeoPixel.h>

//********************** Define sounds *****************************
#define WELCOME     "1.wav"
#define FAILURE1    "coat16.wav"      // failure 1
#define FAILURE2    "shoot16.wav"     // failure 2
#define FAILURE3    "newstk16.wav"    // failure 3
#define HAPPYCHICKEN   "almost16.wav"    // almost done
#define VICTORY     "victor16.wav"   // victory song

//********************** Define sounds end *****************************

//********************** Settings buttons ******************************
ezButton CHICKEN_SWITCH_2(14); // Instance of the button.
ezButton CHICKEN_SWITCH_1(15); // Instance of the button.
ezButton FOX_SWITCH_2(16); // Instance of the button.
ezButton FOX_SWITCH_1(17); // Instance of the button.
ezButton GRAIN_SWITCH_2(18); // Instance of the button.
ezButton GRAIN_SWITCH_1(19); // Instance of the button.
ezButton BOAT_SWITCH(2); // Instance of the button.

//************************** settings buttons end **************************

//********************** Settings Pixelleds ******************************
#define PIN      6 // Which pin on the Arduino is connected to the NeoPixels?
#define NUMPIXELS 15 // How many NeoPixels are attached to the Arduino?
#define FOX_LED_1 0
#define FOX_LED_2  12
#define CHICKEN_LED_1  1
#define CHICKEN_LED_2  13

#define GRAIN_LED_1  2
#define GRAIN_LED_2  14
#define SIDE1   3
#define MIDRIVER 6
#define SIDE2   11
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
//********************** Settings Pixelleds  end******************************

//********************** Settings audio and SD CARD ******************************
#define SD_ChipSelectPin 8  //using digital pin 4 on arduino nano 328
TMRpcm Sound;   // create an object for use in this sketch
#define SPEAKERPIN  9 //connect a speaker between this pin and ground
int volset = 5;
//********************** Settings audio and SD CARD END******************************

// our pions
int CHICKENPos = 1;
int GRAINPos = 1;
int FOXPos = 1;
int BoatPos = 1;
// ********************* start of setup function ******************************
void setup() {
  Serial.begin(115200);
  Sound.speakerPin = SPEAKERPIN; //connect a speaker between this pin and ground

  // Setup the switches debounce time
  CHICKEN_SWITCH_1.setDebounceTime(50);
  CHICKEN_SWITCH_2.setDebounceTime(50);
  FOX_SWITCH_1.setDebounceTime(50);
  FOX_SWITCH_2.setDebounceTime(50);
  GRAIN_SWITCH_1.setDebounceTime(50);;
  GRAIN_SWITCH_2.setDebounceTime(50);
  BOAT_SWITCH.setDebounceTime(50);


  // Setup the SD CARD
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");
    return;   // don't do anything more if not
  }
  Serial.println("SD ok");
  Sound.setVolume(volset);
  Sound.play(WELCOME); //the sound file "1" will play each time the arduino powers up, or is reset

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.setBrightness(20);
  pixels.clear();
  pixels.show();   // Send the updated pixel colors to the hardware.

  Start_position(); //Prepair the game for first use
  delay(1000);

}

void(* resetFunc) (void) = 0; //declare reset function @ address 0


// ********************* start of setup function ******************************

// ********************* Start of main loop function **************************
void loop() {
  // button.read(); // Continuously read the status of the button.
  CHICKEN_SWITCH_1.loop();
  CHICKEN_SWITCH_2.loop();
  FOX_SWITCH_1.loop();
  FOX_SWITCH_2.loop();
  GRAIN_SWITCH_1.loop();
  GRAIN_SWITCH_2.loop();
  BOAT_SWITCH.loop();


  if (CHICKEN_SWITCH_1.isReleased()) {
    // first check boat position. do noting is boat is on other side
    // Serial.println("switch CHICKEN 1 was presses");
    if ((BoatPos == 1) & (CHICKENPos == 1)) {
      //   Serial.println("Boat is ready");
      // Sound.play("1.wav");
      pixels.setPixelColor(CHICKEN_LED_1 , pixels.Color(0, 0, 0)); //dim the CHICKEN led
      pixels.show();
      CHICKENPos = 0; // CHICKEN in the boat
      Cross_1_to_2();
      CHICKENPos = 2; // CHICKEN on other side
      BoatPos = 2; // So is the boat
      pixels.setPixelColor(CHICKEN_LED_2 , pixels.Color(0, 150, 0)); // activate the led CHICKEN 2
      pixels.show();
      if ((CHICKENPos == 2) & (FOXPos == 2) & (GRAINPos == 2))VictoryDance();
    }
    else Serial.println("Boat is not here");

  }


  if (CHICKEN_SWITCH_2.isReleased()) {
    // first check boat position. do noting is boat is on other side
    // Serial.println("switch CHICKEN 2 was presses");
    if ((BoatPos == 2) & (CHICKENPos == 2)) {
      //  Serial.println("Boat is ready");
      // Sound.play("1.wav");
      pixels.setPixelColor(CHICKEN_LED_2 , pixels.Color(0, 0, 0)); //dim the CHICKEN led
      pixels.show();
      CHICKENPos = 0; // CHICKEN in the boat
      Cross_2_to_1();
      CHICKENPos = 1; // CHICKEN is back
      BoatPos = 1; // yep boat is back
      pixels.setPixelColor(CHICKEN_LED_1 , pixels.Color(0, 150, 0)); // activate the led CHICKEN 2
      pixels.show();
    }
    else Serial.println("Boat is not here");
  }

  if (FOX_SWITCH_1.isReleased()) {
    // first check boat position. do noting is boat is on other side
    //  Serial.println("switch FOX 1 was presses");
    if ((BoatPos == 1) & (FOXPos == 1)) {
      //    Serial.println("Boat is ready");
      // Sound.play("1.wav");
      pixels.setPixelColor(FOX_LED_1 , pixels.Color(0, 0, 0)); //dim the CHICKEN led
      pixels.show();
      FOXPos = 0; // FOX in the boat
      Cross_1_to_2();
      FOXPos = 2; // FOX is on other side
      BoatPos = 2; // yep boat is back
      pixels.setPixelColor(FOX_LED_2 , pixels.Color(0, 150, 0)); // activate the led CHICKEN 2
      pixels.show();
      if ((CHICKENPos == 2) & (FOXPos == 2) & (GRAINPos == 2))VictoryDance();
    }
    else Serial.println("Boat is not here");
  }

  if (FOX_SWITCH_2.isReleased()) {
    // first check boat position. do noting is boat is on other side
    //    Serial.println("switch FOX 2 was presses");
    if ((BoatPos == 2) & (FOXPos == 2)) {
      //     Serial.println("Boat is ready");
      // Sound.play("1.wav");
      pixels.setPixelColor(FOX_LED_2 , pixels.Color(0, 0, 0)); //dim the CHICKEN led
      pixels.show();
      FOXPos = 0; // FOX in the boat
      Cross_2_to_1();
      FOXPos = 1; // FOX is back
      BoatPos = 1; // yep boat is back
      pixels.setPixelColor(FOX_LED_1 , pixels.Color(0, 150, 0)); // activate the led CHICKEN 2
      pixels.show();
    }
    else Serial.println("Boat is not here");
  }


  if (GRAIN_SWITCH_1.isReleased()) {
    // first check boat position. do noting is boat is on other side
    //  Serial.println("switch GRAIN 1 was presses");
    if ((BoatPos == 1) & (GRAINPos == 1)) {
      //    Serial.println("Boat is ready");
      // Sound.play("1.wav");
      pixels.setPixelColor(GRAIN_LED_1 , pixels.Color(0, 0, 0)); //dim the CHICKEN led
      pixels.show();
      GRAINPos = 0; // FOX in the boat
      Cross_1_to_2();
      GRAINPos = 2; // GRAIN is on other side
      BoatPos = 2; // yep boat is there too
      pixels.setPixelColor(GRAIN_LED_2 , pixels.Color(0, 150, 0)); // activate the led CHICKEN 2
      pixels.show();
      if ((CHICKENPos == 2) & (FOXPos == 2) & (GRAINPos == 2))VictoryDance();
    }
    else Serial.println("Boat is not here");
  }
  if (GRAIN_SWITCH_2.isReleased()) {
    // first check boat position. do noting is boat is on other side
    //  Serial.println("switch GRAIN 2 was presses");
    if ((BoatPos == 2) & (GRAINPos == 2)) {
      //   Serial.println("Boat is ready");
      // Sound.play("1.wav");
      pixels.setPixelColor(GRAIN_LED_2 , pixels.Color(0, 0, 0)); //dim the CHICKEN led
      pixels.show();
      GRAINPos = 0; // GRAIN in the boat
      Cross_2_to_1();
      GRAINPos = 1; // GRAIN is on other side
      BoatPos = 1; // yep boat is back too
      pixels.setPixelColor(GRAIN_LED_1 , pixels.Color(0, 150, 0)); // activate the led CHICKEN 2
      pixels.show();
    }
    else Serial.println("Boat is not here");
  }
  if (BOAT_SWITCH.isReleased()) {
    //   Serial.println("boat loop called");
    Sound.play("1.wav");
    if (BoatPos == 1) {
      //  Serial.println("Boat pos=1");
      Cross_1_to_2();
      BoatPos = 2;
    }
    else if (BoatPos == 2) {
      // Serial.println("Boat pos=2");
      Cross_2_to_1();
      BoatPos = 1;
    }
    if ((CHICKENPos == 2) & (FOXPos == 2) & (GRAINPos == 2))VictoryDance();
  }

} // bracket from main loop()

// added functions

void Start_position()
{ //Initialize to default settings
  CHICKENPos = 1;
  GRAINPos = 1;
  FOXPos = 1;
  BoatPos = 1;
  pixels.clear();
  pixels.setPixelColor(CHICKEN_LED_1 , pixels.Color(0, 150, 0));
  pixels.setPixelColor(FOX_LED_1 , pixels.Color(0, 150, 0));
  pixels.setPixelColor(GRAIN_LED_1, pixels.Color(0, 150, 0));
  pixels.setPixelColor(SIDE1, pixels.Color(0, 150, 0));
  pixels.show();
}


void Cross_1_to_2()
{
  for (int cnt = SIDE1; cnt < SIDE2; cnt++)
  {
    // dimm old light and enable next one
    pixels.setPixelColor(cnt, pixels.Color(0, 0, 0)); pixels.show();
    delay(200);
    pixels.setPixelColor(cnt + 1, pixels.Color(100, 0, 100)); pixels.show();
    delay(200);
    if (cnt == MIDRIVER)Check(); // all items safe?
  }
}

void Cross_2_to_1()
{
  for (int cnt = SIDE2; cnt > SIDE1; cnt--)
  {
    // dimm old light and enable next one
    pixels.setPixelColor(cnt, pixels.Color(0, 0, 0)); pixels.show();
    delay(200);
    pixels.setPixelColor(cnt - 1, pixels.Color(100, 0, 100)); pixels.show();
    delay(200);
    if (cnt == MIDRIVER + 2)Check(); // all items safe?
  }
}

void Check()
{ // check if the onces left behind are safe
  delay(1500); // first pause the boat
  if ((FOXPos == 1) & (CHICKENPos == 1)) {
    //  Serial.println("FOX eats CHICKEN");  // if FOX1=true and CHICKEN1=true then problem
    Sound.play(FAILURE1);
    for (int fail = 0; fail < 10; fail++) {
      pixels.setPixelColor(CHICKEN_LED_1 , pixels.Color(255, 0, 0)); //dim the CHICKEN led
      pixels.setPixelColor(FOX_LED_1 , pixels.Color(255, 0, 0)); //dim the CHICKEN led
      pixels.show();
      delay(300);
      pixels.clear();
      pixels.show();
      delay(300);
    }

    // delay(1000);
    resetFunc();
  }
  else if ((FOXPos == 2) & (CHICKENPos == 2)) {
    //  Serial.println("FOX eats CHICKEN"); // if FOX2=true and CHICKEN2=true then problem
    Sound.play(FAILURE3);
    for (int fail = 0; fail < 10; fail++) {
      pixels.setPixelColor(CHICKEN_LED_2 , pixels.Color(255, 0, 0)); //dim the CHICKEN led
      pixels.setPixelColor(FOX_LED_2 , pixels.Color(255, 0, 0)); //dim the CHICKEN led
      pixels.show();
      delay(300);
      pixels.clear();
      pixels.show();
      delay(300);
    }

    // delay(1000);
    resetFunc();
  }
  else if ((CHICKENPos == 1) & (GRAINPos == 1)) {
    //  Serial.println("CHICKEN eats GRAIN");  // if CHICKEN1=true and GRAIN1=true then problem
    Sound.play(FAILURE2);
    for (int fail = 0; fail < 10; fail++) {
      pixels.setPixelColor(CHICKEN_LED_1 , pixels.Color(255, 0, 0)); //dim the CHICKEN led
      pixels.setPixelColor(GRAIN_LED_1 , pixels.Color(255, 0, 0)); //dim the CHICKEN led
      pixels.show();
      delay(300);
      pixels.clear();
      pixels.show();
      delay(300);
    }

    // delay(1000);
    resetFunc();
  }
  else if ((CHICKENPos == 2) & (GRAINPos == 2)) {
    //   Serial.println("CHICKEN eats GRAIN"); // if CHICKEN 2=true and GRAIN2=true then problem
    Sound.play(FAILURE3);
    for (int fail = 0; fail < 10; fail++) {
      pixels.setPixelColor(CHICKEN_LED_2 , pixels.Color(255, 0, 0)); //dim the CHICKEN led
      pixels.setPixelColor(GRAIN_LED_2 , pixels.Color(255, 0, 0)); //dim the CHICKEN led
      pixels.show();
      delay(300);
      pixels.clear();
      pixels.show();
      delay(300);
    }

    // delay(1000);
    resetFunc();
  }
  else {
    // Serial.println("No problem");
    Sound.play(HAPPYCHICKEN);
  }
}

void VictoryDance() {
  // whenever the boat is on side two, check where all objects are and do victory dance if needed
  Serial.println("We have ourselves a Winner!!!");
  Sound.play(VICTORY);
  pixels.setBrightness(50);
  for (int cntloop = 0; cntloop < 5; cntloop++) {
    for (int cnt = 0; cnt < NUMPIXELS; cnt++)
    {

      pixels.setPixelColor(cnt, pixels.Color(0, 0, 0)); pixels.show();
      delay(50);
      pixels.setPixelColor(cnt + 1, pixels.Color(0, 200, 0)); pixels.show();
      delay(50);
      // if (cnt == MIDRIVER + 2)Check(); // all items safe?
    }

    for (int cnt2 = NUMPIXELS; cnt2 > 0; cnt2--)
    {

      pixels.setPixelColor(cnt2, pixels.Color(0, 0, 0)); pixels.show();
      delay(50);
      pixels.setPixelColor(cnt2 - 1, pixels.Color(0, 200, 0)); pixels.show();
      delay(50);
      // if (cnt2 == MIDRIVER + 2)Check(); // all items safe?
    }


  }
  pixels.setBrightness(10);
  Start_position();
}
