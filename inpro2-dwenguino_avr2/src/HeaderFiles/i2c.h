#ifndef I2C_H
#define I2C_H

#define LEFT  1
#define UP 2
#define RIGHT  3
#define DOWN 0
#define NONE 5

#define SLAR 0b11010001
#define SLAW 0b11010000

void ERROR(int i);
void initI2C();
void startI2C();
void sendSTART();
void sendRepSTART();
void sendSTOP();
void loadSLAR();
void loadSLAW();
void loadData(int data);
char read(char reg);

void configAccel();

int readAccelX();
int readAccelY();
int readAccelZ();

int readGyroX();
int readGyroY();
int readGyroZ();

int nrmlData(int data);
double*  convToAngle(double* xyz);

int giveDirection(int xangle,int yangle);

//void initAccelerometer();

#endif
