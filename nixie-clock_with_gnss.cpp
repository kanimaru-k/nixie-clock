//http://www.d-b.ne.jp/~k-kita/arduino_105.html
//https://os.mbed.com/teams/CORE/code/Sample_GPS/file/ccc66c37d3f1/main.cpp/

#include "mbed.h"
#include "Serial.h"

#define TIMEZONE 9

Serial gps(D1, D0);       // tx, rx
Serial pc(USBTX, USBRX);    // tx, rx
int k,rlock,mode;
char gps_data[256];
char ns,ew;
float w_time,hokui,tokei;
float g_hokui,g_tokei;
float d_hokui,m_hokui,d_tokei,m_tokei;
unsigned char c;

int gpstime,h,m;

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

void display_numbers(int H,int M) {
  //H=Hour;
  //m=min;
  for (int i = 0; i < 2; i++) {
    Anode_Pins[i]=1;
    Num_Point(M%10);
    wait_ms(3);
    Anode_Pins[i]=0;
    wait_ms(1);
    M = M / 10;
  }
  for (int i = 2; i < 4; i++) {
    Anode_Pins[i]=1;
    Num_Point(H%10);
    wait_ms(3);
    Anode_Pins[i]=0;
    wait_ms(1);
    H = H / 10;
  }
}

void getGPS() {
  c = gps.getc();
  if( c=='$' || k == 256){
    mode = 0;
    k = 0;
    for(int j=0; j<256; j++){
        gps_data[j]=NULL;
    }
  }
  if(mode==0){
    if((gps_data[k]=c) != '\r'){
      k++;
    }else{
      gps_data[k]='\0';
      
      if( sscanf(gps_data, "$GPGGA,%f,%f,%c,%f,%c,%d",&w_time,&hokui,&ns,&tokei,&ew,&rlock) >= 1){
        if(rlock==1 || rlock==1 || rlock==2){
          //pc.printf("Status:Lock(%d)\n\r",rlock);
          
          /*
          w_time += 90000;
          int a = w_time;
          */
          
          /*
          //logitude
          d_tokei= int(tokei/100);
          m_tokei= (tokei-d_tokei*100)/60;
          g_tokei= d_tokei+m_tokei;
          //Latitude
          d_hokui=int(hokui/100);
          m_hokui=(hokui-d_hokui*100)/60;
          g_hokui=d_hokui+m_hokui;
          //pc.printf("Lon:%.6f, Lat:%.6f\n\r",g_tokei, g_hokui);
          */
          gpstime=(w_time/100);     //while(1)からこっちに移した
          h = gpstime/100 + TIMEZONE;
          if (h>=24) h-=24;
          m = gpstime % 100;
          
        }
        else{
          //pc.printf("\n\rStatus:unLock(%d)\n\r",rlock);
          //pc.printf("%s\n\r",gps_data);
        }
        //sprintf(gps_data, "");
      }//if
    }
  }
}

int main(){
    pc.printf("*** GPS GT-720F ***");
    gps.baud(9600);
    //  pc.baud(115200);
    gps.attach(getGPS,Serial::RxIrq);
    
    while(1){
        /*  getgps()に移した
        gpstime=(w_time/100);
        h = gpstime/100 + TIMEZONE;
        if (h>24) h-=24;
        m = gpstime % 100;
        */
        //pc.printf("%d,Hour%d,Min%d\n\r",gpstime,h,m);
        
        display_numbers(h,m);
        }
    
}
