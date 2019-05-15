#include "mbed.h"

DigitalOut Anode_Pins[4]={
    DigitalOut(D12),
    DigitalOut(D11),
    DigitalOut(D10),
    DigitalOut(D9),
};

DigitalOut K155ID1_Pins[4]={
    DigitalOut(D2),
    DigitalOut(D3),
    DigitalOut(D4),
    DigitalOut(D5),
};

const int Num_Array[10][4]{
  {0,0,0,0},//0
  {1,0,0,0},//1
  {0,1,0,0},//2
  {1,1,0,0},//3
  {0,0,1,0},//4
  {1,0,1,0},//5
  {0,1,1,0},//6
  {1,1,1,0},//7
  {0,0,0,1},//8
  {1,0,0,1}//9
};

void Num_Point(int Number){
  for(int w=0; w<4; w++){
    K155ID1_Pins[w] = Num_Array[Number][w];
  }
}

void display_numbers(int m){
  //int m=numbers_to_display;
  for(int i=0; i<4; i++){
    Anode_Pins[i]=1;
    Num_Point(m%10);
    wait_ms(2);
    Anode_Pins[i]=0;
    wait_ms(1);
    m=m/10;
  }
}

int counts = 0;

Ticker ticker;

void count(){
    counts++;
    }

int main() {
    ticker.attach_us(&count, 10000);
    
    while(1) {
        display_numbers(counts);
        if(counts >= 10000) counts=0;
    }
}
