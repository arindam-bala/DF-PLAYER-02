//LAST UPPDATED BUG FREE
//WITHOUT BATTERY CODE
//WITHOUT ON/OFF CODE
//WITHOUT DISPLAY UNIT

#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <EEPROM.h>

#define ACTIVATED LOW

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

//BUTTONS
const int playPause = 4;
const int fastNext = 7;
const int fastPrev = 3;
const int volUp = 8;
const int volDn = 2 ;
const int eqMode = 6;

//FLAGS
boolean playFlag = true;
boolean fastFlag = false;
int eqFlag = 0;

//DATA_HOLDERS
int fileCount;
int currFile = EEPROM.read(1);
int copyTrack;
int currTrack;
int volIndex = EEPROM.read(2);
int eqIndex = EEPROM.read(3);

//COUNTERS
int i; //Track counter

//FUNCTION DECLARARIONS=>
void plAY();
void paUSE();
void fastNEXT();
void fastPREV();
void playBack();
void volUP();
void volDN();
void eQLZR();
//----------------------|

void setup(){
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));

  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms



  //DEFAULT SETUP
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);
  fileCount = myDFPlayer.readFileCounts();
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.enableDAC();

  //PINMODE SETUP
  pinMode(playPause, INPUT_PULLUP);
  pinMode(fastNext, INPUT_PULLUP);
  pinMode(fastPrev, INPUT_PULLUP);
  pinMode(volUp, INPUT_PULLUP);
  pinMode(volDn, INPUT_PULLUP);
  pinMode(eqMode, INPUT_PULLUP);


  //DEFAULT FILE ADJUSTMENT
  if(currFile > fileCount){
    currFile = 1;
  }
  if(currFile < 1){
    currFile = fileCount;
  }
  currTrack = currFile;

  //DEFAULT VOLUME ADJUSTMENT
  if(volIndex >= 30){
    volIndex = 15;
  }
  if(volIndex <= 15){
    volIndex == 15;
  }
  myDFPlayer.volume(volIndex);  //Set volume value (15)

   myDFPlayer.play(currFile);
   delay(200);
   Serial.println(currFile);
   playFlag = true;

}

//-------------|
//LOOP BEGINS=>
void loop(){
  //--------------------------------------------------------------|
  if((digitalRead(playPause) == ACTIVATED) && playFlag == false){
    delay(300);
    plAY();   //Play Function Call
  }
  //--------------------------------------------------------------|
  if((digitalRead(playPause) == ACTIVATED) && playFlag == true){
    delay(300);
    paUSE();  //Pause Function Call
  }
  //--------------------------------------------------------------|
  if(digitalRead(fastNext) == ACTIVATED){
    delay(300);
    fastNEXT();   //Next Function Call
    }
  //--------------------------------------------------------------|
  if(digitalRead(fastPrev) == ACTIVATED){
    delay(300);
    fastPREV();   //Previous Function Call
  }
  //--------------------------------------------------------------|
  if((digitalRead(fastNext)!= ACTIVATED) && (digitalRead(fastPrev)!= ACTIVATED) && fastFlag == true){
    delay(100);
    playBack();   //PlayBack Function Call
  }
  //--------------------------------------------------------------|
  if(digitalRead(volUp) == ACTIVATED){
    delay(300);
    volUP();  //Volume UP Function Call
  }
  //--------------------------------------------------------------|
  if(digitalRead(volDn) == ACTIVATED){
    delay(300);
    volDN();    //Volume DOWN Function Call
  }
  //--------------------------------------------------------------|
  if(digitalRead(eqMode) == ACTIVATED){
    delay(300);
    eQLZR();
  }
  delay(50);
}
//END OF LOOP
//-----------|

//******************************************************************|
//******************************************************************|

//USER DEFINED FUNCTIONS=>
//-------------------------------------------------
//Play Function=>
void plAY() {
  Serial.println("PLAYING");
  myDFPlayer.start();
  delay(10);
  playFlag = true;
}
//-------------------------------------------------
//Pause Function=>
void paUSE() {
  Serial.println("PAUSED");
  myDFPlayer.pause();
  delay(10);
  playFlag = false;
}
//-------------------------------------------------
//Next Function=>
void fastNEXT() {
  currTrack = currTrack + 1;
  for(i = currTrack; i < (currTrack+1); ++i){
    if(i > fileCount){
      currTrack = 1;
      copyTrack = 1;
    }else{
      copyTrack = i;
    }
    currFile = copyTrack;
    fastFlag = true;
    Serial.println(currFile);
    EEPROM.write(1, currFile);
  }
}
//-------------------------------------------------
//Previous Function=>
void fastPREV() {
  currTrack = currTrack - 1;
  for(i = currTrack; i > (currTrack-1); --i){
    if(i < 1){
      currTrack = fileCount;
      copyTrack = fileCount;
    }else{
      copyTrack = i;
    }
  }
  currFile = copyTrack;
  fastFlag = true;
  Serial.println(currFile);
  EEPROM.write(1, currFile);
}
//-------------------------------------------------
//PlayBack Function=>
void playBack() {
  myDFPlayer.play(currFile);
  fastFlag = false;
}
//-------------------------------------------------
//Volume UP Function
void volUP(){
  if(volIndex <=30){
    volIndex = volIndex + 1;
    if(volIndex > 20){
      if(volIndex > 30){
        volIndex = 0;
      }
      Serial.println("HI-VOLUME");
    }
  }
  myDFPlayer.volume(volIndex);
  delay(10);
  Serial.println(volIndex);
  EEPROM.write(2,volIndex);
}
//------------------------------------------------
//Volume DOWN Function
void volDN() {
  if((volIndex <= 30) && (volIndex > 0)){
    volIndex = volIndex - 1;
     myDFPlayer.volume(volIndex);
     delay(10);
     if(volIndex == 0){
        volIndex = 15;
        myDFPlayer.volume(volIndex);
     }
  }
  Serial.println(volIndex);
  EEPROM.write(2, volIndex);
}
//------------------------------------------------
//Equilizer Function
void eQLZR() {
  eqFlag = eqFlag + 1;
  switch(eqFlag){
    case 1:
       myDFPlayer.EQ(DFPLAYER_EQ_POP);
       Serial.println("ROCK");
       break;
    case 2:
       myDFPlayer.EQ(DFPLAYER_EQ_ROCK);
       Serial.println("VOCAL");
       break;
    case 3:
       myDFPlayer.EQ(DFPLAYER_EQ_JAZZ);
       Serial.println("MID");
       break;
    case 4:
       myDFPlayer.EQ(DFPLAYER_EQ_CLASSIC);
       Serial.println("SOFT-BASS");
       break;
    case 5:
       myDFPlayer.EQ(DFPLAYER_EQ_BASS);
       Serial.println("LOUD");
       break;
    case 6:
       myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
       Serial.println("NORMAL");
       eqFlag = 0;
       break;

    default:
        break;
  }
}
