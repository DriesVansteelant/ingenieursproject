#include "HeaderFiles/hulpfuncties.h"
/*Kopieert inhoud van A naar B en zet A op 0.*/
void Swap(BYTE A[8],BYTE B[8]){
  for(int i = 0;i<8;i++){
    B[i] = A[i];
    A[i] = B0;
  }
}

int GiveIndex(BYTE bal[8],int dir){
  int res = -1;
  if((dir==DOWN)||(dir==UP)){
    for(int i=7;i>=0;i--){
      if(bal[i]!=B0){
        res = i;
        break;
      }
    }
  }
  else{
    for(int i=0;i<8;i++){
      if(bal[i]!=B0){
        res = i;
        break;
      }
    }
  }
  return res;
}
int MoveLeft(BYTE balExtra[8],BYTE bal[8]){
  int res = 0;
  int indexD = GiveIndex(bal,LEFT);
  int indexU = GiveIndex(bal,UP);
  int check = bal[indexD];
  for(int i=0;i<8;i++){
    bal[i] = (BYTE) (bal[i] << 1);
    if(check>=7){
        bal[i] = bal[i] & 0b11110;
    }
  }
  if(check>=16){
    for(int i=indexD;i<=indexU;i++){
      if(indexD<=7){
        balExtra[i] = ~bal[i];
        balExtra[i] = (BYTE) ((unsigned int)balExtra[i] >> 2);

        balExtra[i] = balExtra[i] & 0b00111;
      }
    }
    if(check==16){
      for(int i=indexD;i<=indexU;i++){
        if(indexD<=7){
          bal[i]=0b00111;
        }
      }
      for(int i=0;i<8;i++){
        balExtra[i] = B0;
      }
      res = 1;
    }
  }
  return res;
}

int MoveRight(BYTE balExtra[8],BYTE bal[8]){
  int res = 0;
  int indexD = GiveIndex(bal,RIGHT);
  int indexU = GiveIndex(bal,UP);
  int check = bal[indexD];
  for(int i=0;i<8;i++){
    bal[i] = (BYTE) (bal[i]  >> 1);
  }
  if(check<=7){
    for(int i = indexD;i<=indexU;i++){
      if(i<=7){
        balExtra[i] = ~bal[i];
        balExtra[i] = (BYTE) (balExtra[i] << 2);
      }
    }
    if(check == 1){
      for(int i=indexD;i<=indexU;i++){
        if(i<=7){
          bal[i] = 0b11100;
        }
      }
      for(int i=0;i<8;i++){
        balExtra[i] = B0;
      }
      res = 1;
    }
  }
  return res;
}

void MoveDown(BYTE balExtra[8],BYTE bal[8]){
  int indexD = GiveIndex(bal,DOWN);
  int indexU = GiveIndex(bal,RIGHT);   //Geeft de eerste gevulde rij terug
  if(indexD == 7){
    if(indexU==7){
      //cursor opschuiven
      balExtra[0] = bal[indexU];
      balExtra[1] = bal[indexU];
      balExtra[2] = bal[indexU];
      bal[indexU] = B0;
    }
    if(indexU==6){
      balExtra[0] = bal[indexU];
      balExtra[1] = bal[indexU];
      bal[indexU] = B0;
    }
    if(indexU==5){
      balExtra[0] = bal[indexU];
      bal[indexU] = B0;
    }
  }
  else{
    bal[indexD+1] = bal[indexD];
    bal[indexU] = B0;
  }
}

void MoveUp(BYTE balExtra[8],BYTE bal[8]){
  int indexU = GiveIndex(bal,UP);
  int indexD = GiveIndex(bal,RIGHT);
  if(indexD==0){
    if(indexU==2){
      balExtra[7] = bal[indexU];
      bal[indexU] = B0;
    }
    if(indexU==1){
      balExtra[6] = bal[indexU];
      balExtra[7] = bal[indexU];
      bal[indexU] = B0;
    }
    if(indexU==0){
      balExtra[5] = bal[indexU];
      balExtra[6] = bal[indexU];
      balExtra[7] = bal[indexU];
      bal[indexU] = B0;
    }
  }
  else{
    bal[indexD-1] = bal[indexD];
    bal[indexU] = B0;
  }
}

void PrintBal(BYTE bal[8],int CursorH,int CursorB, int index){
  installLCD(bal, 0x40+(8*(index-1)));
  printCharToLCD(index-1, CursorH,CursorB);
}
