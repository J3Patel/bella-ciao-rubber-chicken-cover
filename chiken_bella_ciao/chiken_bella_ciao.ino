// Play a file from the SD card in looping mode, from the SD card.
// Example program to demonstrate the use of the MIDFile library
//
// Hardware required:
//  SD card interface - change SD_SELECT for SPI comms

#include <SdFat.h>
#include <MD_MIDIFile.h>
#include <arduino-timer.h>

auto timer = timer_create_default(); // create a timer with default settings


#define DEBUGS(s)     Serial.print(s)
#define DEBUG(s, x)   { Serial.print(F(s)); Serial.print(x); }
#define DEBUGX(s, x)  { Serial.print(F(s)); Serial.print(x, HEX); }
#define SERIAL_RATE 57600

#define  SD_SELECT  4

//#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))
//bella_ciao_chicken_fast
const char *loopfile = "bella_ciao_chicken_fast.mid";  // simple and short file
//blvrnew
//blvrnewloop
SdFat	SD;
MD_MIDIFile SMF;
//#define DUMP_DATA 1;


bool started = false;

void play(int [], bool, int);

void midiCallback(midi_event *pev)
{
  
  DEBUGX("\nM T", pev->track);
  DEBUG(":  Ch ", pev->channel);
  DEBUGX(" Data", sizeof(pev->data));
  DEBUGX(" Data0", pev->data[0]);
  DEBUGX(" Data1", pev->data[1]);
  DEBUGX(" Data2", pev->data[2]);
//
  int note = pev->data[1];
  int velocity = pev->data[2];
  int mDelay = pev->channel;
//
  DEBUG(" ", mDelay);
  DEBUG(" ", note);
  DEBUG(" ", velocity);
  Serial.println("--");

  switch(note) {
    case 69: // A5
    play(2, velocity != 0);
    break;
    case 74://D6
    play(3, velocity != 0);
    break;

    case 76://E6
    play(4, velocity != 0);
    break;

    case 77: //F6
    play(5, velocity != 0);
    break;


    case 79://G6
    play(6, velocity != 0);
    break;

    case 81://A6
    play(7, velocity != 0);
    break;

    case 82: // A#6
    play(8, velocity != 0);
    break;

    
    default:
      Serial.println("NOTHING");
    break;

  }
}


void play(int note, bool on) {

  if (on) {

      digitalWrite(note, LOW);
  } else {
      digitalWrite(note, HIGH);
  }
//  delay(50);
}

void sysexCallback(sysex_event *pev)
// Called by the MIDIFile library when a system Exclusive (sysex) file event needs 
// to be processed through the midi communications interface. Most sysex events cannot 
// really be processed, so we just ignore it here.
// This callback is set up in the setup() function.
{
  DEBUGS("\nS T");
  DEBUGS(pev->track);
  DEBUGS(": Data ");
  for (uint8_t i=0; i<pev->size; i++)
  {
    DEBUGS(pev->data[i]);
//    DEBUG(' ');
  }
}

void setup(void)
{
  
  int  err;
  Serial.begin(SERIAL_RATE);
  DEBUGS("\n[MidiFile Looper]");

  // Initialize SD
  if (!SD.begin(SD_SELECT, SPI_FULL_SPEED))
  {
    DEBUGS("\nSD init fail!");
    while (true) ;
  }

  // Initialize MIDIFile
  SMF.begin(&SD);
  SMF.setMidiHandler(midiCallback);
//  SMF.looping(true);
  SMF.setTempo(10);
  SMF.setSysexHandler(sysexCallback);

  // use the next file name and play it
  DEBUG("\nFile: ", loopfile);
  SMF.setFilename(loopfile);
  err = SMF.load(loopfile);
  if (err != 0)
  {
    DEBUG("\nSMF load Error ", err);
    while (true);
  }

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  
  
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(8, HIGH);
  


// call the toggle_led function every 500 millis (half second)
  

}


void loop(void)
{
  // play the file
  if (!SMF.isEOF())
  {
    SMF.getNextEvent();
  }

}
