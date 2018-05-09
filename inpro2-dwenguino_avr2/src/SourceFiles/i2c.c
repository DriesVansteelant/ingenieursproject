#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "HeaderFiles/dwenguinoBoard.h"
#include "HeaderFiles/dwenguinoLCD.h"
#include "HeaderFiles/i2c.h"
#include <util/twi.h>


void ERROR(int i){
  printIntToLCD(i, 1,i);
}

void initI2C(){
  PRR0 &= ~_BV(PRTWI);
  TWCR |= _BV(TWEN);


  TWSR &= ~_BV(TWPS0);
  TWSR &= ~_BV(TWPS1);//Prescaler = 1
  TWBR = 0b00001000; //Division factor 8, 4 is (voorlopig) te snel
}

//MASTER RECEIVER
void sendSTART(){
   //zet twi aan in de microcontroller.
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
  //wachten op TWINT (confirmatie startconditie)
  while (!(TWCR & (1<<TWINT)));

  if ((TWSR & 0xF8) != TW_START) ERROR(1);
  //printIntToLCD(TWSR,0,4);
}
void sendRepSTART(){
  TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
  //wachten op TWINT (confirmatie startconditie)
  while (!(TWCR & (1<<TWINT)));

  if ((TWSR & 0xF8) != TW_REP_START) ERROR(1);
}
void sendSTOP(){
  TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

//Zet SLA_R in TWDR en verwijdert TWINT bit in TWCR zodat
//adres doorgestuurd wordt
void loadSLAW(){
  TWDR=SLAW;
  //printIntToLCD(TWDR,0,9);
  TWCR =(1<<TWEN)|(1<<TWINT);
  while (!(TWCR & (1<<TWINT)));//wacht op TWINT
  //printIntToLCD(TWSR,0,8);

  if ((TWSR & 0xF8) != TW_MT_SLA_ACK ) {
    ERROR(TWSR);

  }
}
//stuurt het register van de data door
void loadSLAR(){
  TWDR=SLAR;//register adres in twdr.
  TWCR =(1<<TWEN)|(1<<TWINT);
  while (!(TWCR & (1<<TWINT)));//wacht op TWINT

  if ((TWSR & 0xF8) != TW_MR_SLA_ACK) {
    ERROR(4);
    //printIntToLCD(TWSR,0,8);
  }
}
//stuurt data
void loadData(int data){
  TWDR=data;//reset signaal in twdr.
  TWCR =(1<<TWEN)|(1<<TWINT);
  while (!(TWCR & (1<<TWINT)));//wacht op TWINT

  if ((TWSR & 0xF8) != TW_MT_DATA_ACK) {
    ERROR(4);
    //printIntToLCD(TWSR,0,8);
  }
}



//zet data in TWDR en cleared TWINT in TWDR
//TWCR = (1<<TWINT) |  (1<<TWEN);

void startI2C(int adres){
  initI2C();
  sendSTART();
  loadSLAW();
  loadData(0x6B);//register i.v.m powermanagement (van de accmeter)
  loadData(0b00000000);//signaal restetten
  sendSTOP();

}

//range= +-2^(g+1)
void configAccel(int g){
  sendSTART();
  loadSLAW();
  loadData(0x1C);

  int reg;
  switch (g) {
    case 0: reg=0b00000000;
            break;
    case 1: reg=0b00001000;
            break;
    case 2: reg=0b00010000;
            break;

    case 3: reg=0b00011000;
            break;
  }

  loadSLAW();
  TWDR=reg;
  TWCR =(1<<TWINT)|(1<<TWEN);
  while (!(TWCR & (1<<TWINT)));

  sendSTOP();
}


//leest data uit het meegegeven register
char read(char reg){
  TWCR = 0;
  sendSTART();
  loadSLAW();
  loadData(reg);//addres en register doorgeven

  sendRepSTART(); //repeated start
  loadSLAR();
  TWDR=reg;
  TWCR =(1<<TWINT)|(1<<TWEN);
  while (!(TWCR & (1<<TWINT)));//wacht op TWINT
  sendSTOP();
  return TWDR;
}

int readAccelX(){
  int ret= (int)(read(0x3B)<<8) + read(0x3C);
  return ret;
}

int readAccelY(){
  int ret= (int)(read(0x3D)<<8) + read(0x3E);
  return ret;
}

int readAccelZ(){
  int ret= (int)(read(0x3F)<<8) + read(0x40);
  return ret;
}

int readGyroX(){
  int ret = (int)(read(0x43)*2^8) + read(0x44);
  return ret;
}

int readGyroY(){
  int ret = (int)(read(0x45)*2^8) + read(0x46);
  return ret;
}

int readGyroZ(){
  int ret = (int)(read(0x47)*2^8) + read(0x48);
  return ret;
}

int nrmlData(int data){
  int ret=(int) floor(100*(data*2)/32768);
  return ret;

}

//van data naar hoek
double* convToAngle(double xyz[3]){
  double xa=xyz[0];
  double ya=xyz[1];
  double za=xyz[2];

  double xg=xa*2/pow(2,15);
  double yg=ya*2/pow(2,15);
  double zg=za*2/pow(2,15);

/*
  double xAngle = atan( xa / (sqrt(square(ya) + square(za))));
  double yAngle = atan( ya / (sqrt(square(xa) + square(za))));
  double zAngle = atan( sqrt(square(xa) + square(ya)) / za);
*/

  double xAngle = atan2( xg,yg);
  double yAngle = atan2(-yg,sqrt(pow(zg,2)+pow(xg,2)));

  xAngle *= 180.00;   yAngle *= 180.00;   //zAngle *= 180.00;
  xAngle /= 3.141592; yAngle /= 3.141592; //zAngle /= 3.141592;
/*
  if(yAngle>0) yAngle=90-yAngle;
  else yAngle = -90-yAngle;
*/
  //double * ret=(double*) malloc(3*sizeof(double));
  xyz[0]=xAngle;
  xyz[1]=yAngle;
  //xyz[2]=zAngle;
  return xyz;
}

int giveDirection(int xangle, int yangle){
  if(abs(xangle)>abs(yangle)){
    if(xangle>10000)return RIGHT;
    else if (xangle<-10000) return LEFT;
    else return NONE;
  }
  else {
    if(yangle>10000)return UP;
    else if (yangle<-10000) return DOWN;
    else return NONE;
  }
}
