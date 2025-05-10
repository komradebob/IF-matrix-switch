//
// hardware.ino
//
// Code to manipulate the physical hardware
//
// November 8,2024  Bob Bownes
//
//


#include "if_switch.h"

///////////////////////////////////////////////////// Wordset ////////////////////////////////////////////
void wordset(char a){

  bool d0,d1,d2,d3,d4,d5,d6,d7;

  d7=a & 0x80;
  d6=a & 0x40;
  d5=a & 0x20;
  d4=a & 0x10;
  d3=a & 0x08;
  d2=a & 0x04;
  d1=a & 0x02;
  d0=a & 0x01;

  digitalWrite(D0,d0);
  digitalWrite(D1,d1);
  digitalWrite(D2,d2);
  digitalWrite(D3,d3);
  digitalWrite(D4,d4);
  digitalWrite(D5,d5);
  digitalWrite(D6,d6);
  digitalWrite(D7,d7);

}


///////////////////////////////////////////////// Latch /////////////////////////////////
void latch(char which) {
  bool waste;
  switch (which) {
    case 1:
      toggle(SELECT1);
      break;
    case 2:
      toggle(SELECT2);
      break;
    case 3:
      toggle(SELECT3);
      break;
    default:
      waste = 1;
#ifdef DEBUG
      Serial.println(F("OOPS"));
#endif
  }
  return;
}


////////////////////////////////////////////// toggle ////////////////////////////////////////////////
void toggle(char which){
    digitalWrite(which, HIGH);
    digitalWrite(which, LOW);
    return;
}

///////////////////////////////////////////// Cylon ///////////////////////////////////////////////////
void cylon(char which, int howmany){
   unsigned char i,j,out;
   out=0;
   j=0;
while(j<howmany) {
   for (i=0;i<8;i++) {
    out=(char)1<<i;
    wordset(out);
    latch(which);
    delay(25);
   }
   for(i=0;i<8;i++){
    out=(char)0x01 << 8-i;
    wordset(out);
    latch(which);
    delay(25);
   }
   j++;
   }
}


////////////////////////////////////////////////////////////////////////////////////
// put the values of the matrixes into the relays
// We could do this with a loop, but that eats up variable space. This is just code
void updaterelays(){

#ifdef DEBUG
Serial.println("updaterelay");
#endif

  digitalWrite(D0,matrix1state[0]);
  digitalWrite(D1,matrix1state[1]);
  digitalWrite(D2,matrix1state[2]);
  digitalWrite(D3,matrix1state[3]);
  digitalWrite(D4,matrix1state[4]);
  digitalWrite(D5,matrix1state[5]);
  digitalWrite(D6,matrix1state[6]);
  digitalWrite(D7,matrix1state[7]);

  latch(1);

  digitalWrite(D0,matrix1state[8]);
  digitalWrite(D1,matrix1state[9]);
  digitalWrite(D2,matrix1state[10]);
  digitalWrite(D3,matrix1state[11]);
  digitalWrite(D4,matrix1state[12]);
  digitalWrite(D5,matrix1state[13]);
  digitalWrite(D6,matrix1state[14]);
  digitalWrite(D7,matrix1state[15]);

  latch(2);


  digitalWrite(D0,matrix2state[0]);
  digitalWrite(D1,matrix2state[1]);
  digitalWrite(D2,matrix2state[2]);
  digitalWrite(D3,matrix2state[3]);
  digitalWrite(D4,matrix2state[4]);
  digitalWrite(D5,matrix2state[5]);
  digitalWrite(D6,matrix2state[6]);
  digitalWrite(D7,matrix2state[7]);

  latch(3);

}
