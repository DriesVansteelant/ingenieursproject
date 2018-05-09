#include <avr/io.h>
#include <util/delay.h>
#include "HeaderFiles/dwenguinoBoard.h"
#include "HeaderFiles/dwenguinoLCD.h"
#include <avr/interrupt.h>
#include <stdlib.h>
#include "HeaderFiles/hulpfuncties.h"
#include "HeaderFiles/i2c.h"

void MoveBal(int dir);

int dir = DOWN;
int speed = 500;
int CursorB = 0;
int CursorH = 0;
BYTE bal1[8] = {
  0b11100,
  0b11100,
  0b11100,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

BYTE bal2[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

BYTE bal3[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

BYTE bal4[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};


int main(void)
{
initBoard();
initLCD();
backlightOn();

SREG |= _BV(PE7);
DDRE |= _BV(PE4);
PORTE |=_BV(PE4);

//poorten en pull up acc
DDRD |= _BV(PD0);
DDRD |= _BV(PD1);
PORTD |=_BV(PD0);
PORTD |=_BV(PD1);


//startconditie


TWCR &= ~_BV(TWSTO);
TWCR &= ~_BV(TWSTA);

clearLCD();

//loadSLAR();

/*sendSTART();
loadSLAR();*/
//BYTE ball[8] = {0b11100,0b11100,0b11100,0};


//PORTF op output
//DDRF |=_BV(PF0);


startI2C();

//gameloop
while(1){
  _delay_ms(200);
  clearLCD();
  PrintBal(bal1,CursorH,CursorB,1);
  PrintBal(bal2,CursorH,CursorB+1,2);
  PrintBal(bal3,CursorH+1,CursorB,3);
  PrintBal(bal4,CursorH+1,CursorB+1,4);
  dir = giveDirection(readAccelX(),readAccelY());
  //printIntToLCD(dir, 1, 5);
  MoveBal(dir);

}

//customCharToLCD(0x40, bal,0,5);
return 0;
}




void MoveBal(int dir){
  int shift=0;
  switch(dir){
    /*case(NONE):
      break;*/
    case(RIGHT):
      if((GiveIndex(bal1,RIGHT)==-1) && (GiveIndex(bal3,RIGHT)==-1)){
          Swap(bal2,bal1);
          Swap(bal4,bal3);
          CursorB++;
      }
      if(GiveIndex(bal1,RIGHT)!=-1){
        shift = MoveRight(bal2,bal1);
      }
      if(GiveIndex(bal3,RIGHT)!=-1){
        shift = MoveRight(bal4,bal3);
      }
      if(shift){
        CursorB++;
      }
      break;
    case(LEFT):
      if((GiveIndex(bal2,LEFT)==-1) && (GiveIndex(bal4,LEFT)==-1)){
          Swap(bal1,bal2);
          Swap(bal3,bal4);
          CursorB--;
      }
      if(GiveIndex(bal2,LEFT)!=-1){
        shift = MoveLeft(bal1,bal2);
      }
      if(GiveIndex(bal4,LEFT)!=-1){
        shift = MoveLeft(bal3,bal4);
      }
      if(shift){
        CursorB--;
      }
      break;
    case(DOWN):
      if((GiveIndex(bal1,DOWN)==-1) && (GiveIndex(bal2,DOWN)==-1)){
        if(GiveIndex(bal3,DOWN)!=-1){
          MoveDown(bal1,bal3);
        }
        if(GiveIndex(bal4,DOWN)!=-1){
          MoveDown(bal2,bal4);
        }
      }
      else{
        if(GiveIndex(bal1,DOWN)!=-1){
          MoveDown(bal3,bal1);
        }
        if(GiveIndex(bal2,DOWN)!=-1){
          MoveDown(bal4,bal2);
        }
      }
      break;
    case(UP):
    if((GiveIndex(bal3,UP)==-1) && (GiveIndex(bal4,UP)==-1)){
      if(GiveIndex(bal1,UP)!=-1){
        MoveUp(bal3,bal1);
      }
      if(GiveIndex(bal2,UP)!=-1){
        MoveUp(bal4,bal2);
      }
    }
    else{
      if(GiveIndex(bal3,UP)!=-1){
        MoveUp(bal1,bal3);
      }
      if(GiveIndex(bal4,UP)!=-1){
        MoveUp(bal2,bal4);
      }
    }
      break;
    }
  }
