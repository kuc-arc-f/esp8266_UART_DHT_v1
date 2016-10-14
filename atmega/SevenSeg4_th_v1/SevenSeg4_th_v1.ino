#include <DHT.h>

#define DHTPIN 14     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

//define
const int mVoutPin = 0;
const static int mAnode[4] = {9, 10, 11, 12 };
const static int mCathod[7] ={2,3,4,5,6,8,7};

int   mCathod_pin[7]   ={6,4,2,1,0,5,3 };

const int mMaxAnode=4;
int mSTAT =0;
int mSTAT_CONECT=1;
int mSTAT_DISP=2;
int iMode=0;
int mMode_TMP =0;
int mMode_HUM =1;
int mMode_TIME=2;

int mTempNum=0;
int mHumNum=0;

//int mCount= 0;
static uint32_t mTimerTmp;
String mReceive="";

bool Num_Array[10][8]={
// a b c d e f g dp 
{ 1,1,1,1,1,1,0,0}, //0 :0
{ 0,1,1,0,0,0,0,0}, //1 :1
{ 1,1,0,1,1,0,1,0}, //2 :2
{ 1,1,1,1,0,0,1,0}, //3 :3
{ 0,1,1,0,0,1,1,0}, //4 :4
{ 1,0,1,1,0,1,1,0}, //5 :5
{ 1,0,1,1,1,1,1,0}, //6 :6
{ 1,1,1,0,0,0,0,0}, //7 :7
{ 1,1,1,1,1,1,1,0}, //8 :8
{ 1,1,1,1,0,1,1,0}, //9 :9   
};

//
void setup() {
  Serial.begin(9600);
  dht.begin();
  Serial.println("# Start-setup");
  mSTAT =mSTAT_CONECT;
  //cathod
  for(int i= 0 ;i< 7;i++){
    pinMode(mCathod[i], OUTPUT);
  }
  for(int j=0 ;j<  mMaxAnode ; j++){
    pinMode(mAnode[j], OUTPUT);
  }
}

//
void NumPrint(int Number){
//Serial.println(Num_Array[iDeg_1][k]);
  for (int i=0; i< 7; i++){
    int iTmp= mCathod_pin[i];
    if(Num_Array[Number][i]==1){
       digitalWrite(mCathod[iTmp ], LOW);
//Serial.print(mCathod[iTmp ]);
    }
    //else{
    //   digitalWrite(mCathod[iTmp ], HIGH );
    //}
    delayMicroseconds( 500 );
    if(Num_Array[Number][i]==1){
       digitalWrite(mCathod[iTmp ], HIGH);
//Serial.print("0");
    }
//Serial.println("");    
  }
}

void set_anode(int pinAnode){
  for(int i=0;i <mMaxAnode ; i++){
    if(mAnode[i]==pinAnode){
       digitalWrite(mAnode[ i  ], HIGH);
    }
    else{
       digitalWrite(mAnode[ i  ], LOW);
    }
  }
}

//
//void proc_display(int iTemp){
void proc_display(int iTemp, int typMode){
//  int iDeg_1000= iTemp / 1000;
//  int iDeg_100= iTemp % 100;
  int iDeg_1000= 0;
  int iDeg_100= 0;
  
  int iDeg_10 = iTemp / 10;
  int iDeg_1  = iTemp % 10;
//Serial.print("iDeg_10=");
//Serial.println(iDeg_10);
//Serial.print("iDeg_1=");
//Serial.println(iDeg_1 );
//for(int k=0;  k< 7; k++ ){
//  Serial.println(Num_Array[iDeg_1][k]);
//}
//Serial.println("end.d1===");

//d2

   if(typMode==mMode_TMP){
	   for(int i=0;  i< 200; i++ ){
	       // dig-10
	       set_anode(mAnode[1]);
	       NumPrint(iDeg_10);
	       set_anode(mAnode[0]);
	       NumPrint(iDeg_1);       
	   }
   }else{
       for(int j=0; j<3; j++){
	   for(int i=0;  i< 50; i++ ){
	       // dig-10
	       set_anode(mAnode[1]);
	       NumPrint(iDeg_10);
	       set_anode(mAnode[0]);
	       NumPrint(iDeg_1);       
	   }
	   delay(100);
	} 
   }
}
//
long convert_Map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

//
void loop() {
  delay( 100 );
  int iNum=0;
  if (millis() > mTimerTmp) {
    mTimerTmp = millis()+ 3000;

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");
        return;
    }
    int itemp =(int)t;   
    int iHum = (int)h;   
    char sTmp[2+1];
    char sHum[3+1];
    sprintf(sTmp, "%2d", itemp);
Serial.print("dat="); 
Serial.print(sTmp);
//Serial.print("hum="); 
    sprintf(sHum, "%03d", iHum);
Serial.println( sHum );
    if(iMode== mMode_TMP){
      proc_display( itemp, mMode_TMP );
      iMode=mMode_HUM;
    }else{
      proc_display( iHum , mMode_HUM );
      iMode=mMode_TMP;
    }
//    mCount=mCount+1;
  }
}





