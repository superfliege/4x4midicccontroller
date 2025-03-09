#include "MIDIUSB.h"

const int muxSIG3 = A1;
const int muxS03 = 2;
const int muxS13 = 3;
const int muxS23 = 4;
const int muxS33 = 5;
const int entradasmplex3 = 16;
int VAPot = 0; //current value pot
byte VAMidi = 0; //current value midi();

void SetMuxChannel3(byte channel)
{
  digitalWrite(muxS03, bitRead(channel, 0));
  digitalWrite(muxS13, bitRead(channel, 1));
  digitalWrite(muxS23, bitRead(channel, 2));
  digitalWrite(muxS33, bitRead(channel, 3));
}
void setup()
{

  pinMode(muxS03 , OUTPUT);
  pinMode(muxS13, OUTPUT);
  pinMode(muxS23 , OUTPUT);
  pinMode(muxS33 , OUTPUT);
  pinMode(muxSIG3 , INPUT);
  Serial.begin(9600);
  delay(1000);
}

void loop()
{
  static byte lastVAMidi[entradasmplex3] = {0};

  for (int i = 0 ; i < entradasmplex3 ; i++ )
  {
    SetMuxChannel3(i);
    VAPot  = analogRead(muxSIG3);
    VAMidi = map(VAPot, 0, 1023, 0, 127);

    if (abs(VAMidi - lastVAMidi[i]) > 5)
    {
      //send midi
      midiEventPacket_t event = {0x0B, 0xB0, i+100, VAMidi};
      MidiUSB.sendMIDI(event);
      MidiUSB.flush();

      lastVAMidi[i] = VAMidi;
    }

    if (i == 0)
      Serial.print(i);
      Serial.print(".........");
      Serial.println(VAMidi);
    }
}
