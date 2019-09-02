#include <TinyGPSPlus.h>
TinyGPSPlus gps;
#include <SoftwareSerial.h>
#define TxD 3
#define RxD 12
const int rela = 6;
const int relb = 5;
const int relc = 7;
const int greenPin = 4;
const int redPin = 8;
const int bluePin = 9;
const int whitePin = 2;
int inpoint;
int straight;
int xyc = 0;
unsigned long starttime;
unsigned long endtime;
int finalmove;
int irlaiks;
int durration;
int secf;
int satnum;
int fres;
int fresn;
int Rstarted;
int Lstarted;
int gotend;
int endwhile;
int gotdurr;
int BLstarted;
int BRstarted;
int state = 0;
int bckl = 0;
int bckr = 0;
int manorauto = 1;
int candostarttime;
unsigned long lastUpdateTime = 0;
unsigned long delaytime = 0;
int fromandroid = 0;
double distanceToDestination;
char currentstatus;
int currentspeed;
int mazturn = 0;
const char *actionname;
int tdtochange;
int courseChangeNeeded;

SoftwareSerial gpsserial(RxD, TxD);

void setup() {
  gpsserial.begin(9600);
  Serial.begin(115200);
  pinMode(rela, OUTPUT);
  pinMode(relb, OUTPUT);
  pinMode(relc, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(whitePin, OUTPUT);
}
void loop() {
  while (gpsserial.available()) {
    gps.encode(gpsserial.read());
  }
  // Koordinatu array
  double x[] = {56.963645, 666};
  double y[] = {23.690411, 666};

  double des_LAT = x[xyc];
  double des_LNG = y[xyc];
  if (Serial.available()) { // Checks whether data  is comming from the serial port
    fromandroid = Serial.read();
  }
  if (millis() - lastUpdateTime >= 1500) {
    lastUpdateTime = millis();
    distanceToDestination = TinyGPSPlus::distanceBetween(gps.location.lat(), gps.location.lng(), des_LAT, des_LNG);
    double courseToDestination = TinyGPSPlus::courseTo(gps.location.lat(), gps.location.lng(), des_LAT, des_LNG);
    courseChangeNeeded = (int)(360 + courseToDestination - gps.course.deg()) % 360;
    int currentdirection = (int) gps.course.deg() % 360;
    currentspeed = (int)gps.speed.kmph();
    int satelitesno = (int)gps.satellites.value();
    // Total degrees to change
    int tdtochange = (min( ((int)currentdirection - (int)courseToDestination + 360) % 360, ((int)courseToDestination - (int)currentdirection + 360) % 360 ));
    int xycn;
    if(xyc == 0){xycn = 01;}else{xycn = xyc;}
    if(xycn > 0){Serial.println(xycn);}else{Serial.println("");}
    if(tdtochange > 0){Serial.println(tdtochange);}else{Serial.println("");}
    if(currentspeed > 0){Serial.println(currentspeed);}else{Serial.println("");}
    if(distanceToDestination > 0){Serial.println(distanceToDestination);}else{Serial.println("");}
    if(actionname == ""){Serial.println("");}else{Serial.println(actionname);}
  
}
  if ((int)x[xyc] == 666) {
    xyc = 0;
  }
  if (fromandroid == 49) {
    manorauto = 2;
  }
  if (fromandroid == 50) {
    manorauto = 1;
  }

  if (fromandroid == 54) {
    nextpoint();
    setdef();
  }
  if (manorauto == 1) {
  blueoff();
    if (fromandroid == 51) {
      turnleft();
    }
    if (fromandroid == 52) {
      turnright();
    }
    if (fromandroid == 53) {
      stopm();
      setdef();
    }
  }
  if (manorauto == 2) {
    blueon();
    if (currentspeed >= 2) {
      if ((distanceToDestination <= 20.0 && inpoint == 0) || inpoint == 1)
      {
        if (inpoint == 0) {
          delaytime = millis();
        }
        inpoint = 1;
        actionname = "SASNIEGTS";
        whiteon(); xyc++; if (millis() - delaytime > 4000) {
          inpoint = 0;
          delaytime = 0;
        }
      }
      if (millis() > 3000) {

        if (((courseChangeNeeded >= 350 || courseChangeNeeded < 10) && BLstarted == 0 && BRstarted == 0 && Rstarted == 0 && Lstarted == 0 && inpoint == 0) || straight == 1) {
          if (straight == 0) {
            delaytime = millis();
            straight = 1;
          }
          greenon(); redon();
          actionname = " TAISNI";
          if (millis() - delaytime > 3000) {
            straight = 0;
            delaytime = 0;
          }
        }
        if ((courseChangeNeeded >= 10 && courseChangeNeeded < 15 && BLstarted == 0 && BRstarted == 0 && straight == 0 && inpoint == 0) || Rstarted == 1) {
          if (Rstarted == 0) {
            delaytime = millis();
            Rstarted = 1;
          }
          actionname = " mR";
          if (mazturn == 0)
          {
            redon();
            turnright();
          }
          if (millis() - delaytime > 1000) {
            mazturn = 1;
            stopm();
            redoff();
          }
          if (millis() - delaytime > 4000) {
            setdef();
          }
        }
        if ((courseChangeNeeded >= 345 && courseChangeNeeded < 350 && BLstarted == 0 && BRstarted == 0 && straight == 0 && inpoint == 0) || Lstarted == 1) {
          if (Lstarted == 0) {
            delaytime = millis();
            Lstarted = 1;
          }
          actionname = " mL";
          if (mazturn == 0)
          {
            greenon();
            turnleft();
          }
          if (millis() - delaytime > 1000) {
            mazturn = 1;
            stopm();
            greenoff();
          }
          if (millis() - delaytime > 4000) {
            setdef();
          }
        }

        // IF Vajag LIELO PA LABI ---------------------------------------------------------------------------------------------------
        if ((courseChangeNeeded >= 15 && courseChangeNeeded < 180 && BLstarted == 0 && straight == 0 && inpoint == 0) || BRstarted == 1) {
          if (candostarttime == 0) {
            candostarttime = tdtochange;
            BRstarted = 1;
          }
          if (candostarttime != 0 && fresn == 0) {
            turnright(); fresn = tdtochange / 3;
            actionname = " R";
          }

          // Starttime sacies ja ir sakusies laivas griesanas vajadzigaja virsziena
          if (candostarttime > tdtochange && starttime == 0) {
            starttime = millis();
          }

          if (starttime != 0 && endwhile == 0) {
            if (millis() - starttime >= 4000) {
              endwhile = 2;
              actionname = "4000s";
            }
          }
          if (fresn > tdtochange && endwhile == 0) {
            endwhile = 1;
            actionname = "1/3";
          }
          if ((endwhile == 1 || endwhile == 2) && endtime == 0) {
            endtime = millis();
            actionname = "END";
          }
          if (endtime != 0 && gotdurr != 1) {
            gotdurr = 1;
            durration = (endtime - starttime);
          }
          if (gotdurr == 1) {
            if (bckr == 0) {
              delaytime = millis();
              bckr = 1;
            }
            turnleft();
            actionname = "turnback";
            if (millis() - delaytime > durration) {
              setdef();
            }
          }
        }
        // IF Vajag LIELO PA KREISI ---------------------------------------------------------------------------------------------------
        if ((courseChangeNeeded >= 180 && courseChangeNeeded < 345 && BRstarted == 0 && straight == 0 && inpoint == 0) || BLstarted == 1) {
          if (candostarttime == 0) {
            candostarttime = tdtochange;
            BLstarted = 1;
          }

          if (candostarttime != 0 && fresn == 0) {
            turnleft();  fresn = tdtochange / 3;
            actionname = " L";
          }

          // Starttime sacies ja ir sakusies laivas griesanas vajadzigaja virsziena
          if (candostarttime > tdtochange && starttime == 0) {
            starttime = millis();
          }

          if (starttime != 0 && endwhile == 0) {
            if (millis() - starttime >= 4000) {
              endwhile = 2;
              actionname = "4000s";
            }
          }
          if (fresn > tdtochange && endwhile == 0) {
            endwhile = 1;
            actionname = "1/3";
          }
          if ((endwhile == 1 || endwhile == 2) && endtime == 0) {
            endtime = millis();
            actionname = "END";
          }
          if (endtime != 0 && gotdurr == 0) {
            gotdurr = 1;
            durration = (endtime - starttime);
          }
          if (gotdurr == 1) {
            if (bckr == 0) {
              delaytime = millis();
              bckl = 1;
            }
            turnright();
            actionname = "turnback";
            if (millis() - delaytime > durration) {
              setdef();
            }
          }
        }
      }

    } else {
      actionname = "Slow!"; setdef();
    }
    //}
  }

}
void setdef() {
  gotend = 0; gotdurr = 0; endwhile = 0; endtime = 0; BLstarted = 0;
  BRstarted = 0; Rstarted = 0; Lstarted = 0; starttime = 0;  greenoff(); redoff();
  whiteoff(); blueoff();  stopm(); inpoint = 0; candostarttime = 0; delaytime = 0;
  bckl = 0; bckr = 0; mazturn = 0;
}
void nextpoint() {
  xyc++;
}
void greenon() {
  digitalWrite(greenPin, HIGH);
}
void greenoff() {
  digitalWrite(greenPin, LOW);
}
void redon() {
  digitalWrite(redPin, HIGH);
}
void redoff() {
  digitalWrite(redPin, LOW);
}
void blueon() {
  digitalWrite(bluePin, HIGH);
}
void blueoff() {
  digitalWrite(bluePin, LOW);
}
void whiteon() {
  digitalWrite(whitePin, HIGH);
}
void whiteoff() {
  digitalWrite(whitePin, LOW);
}
void stopm() {
  digitalWrite(relc, HIGH);
}
void turnright() {
  redon();
  digitalWrite(relb, LOW);
  digitalWrite(rela, HIGH);
  digitalWrite(relc, LOW);
}
void turnleft() {
  greenon();
  digitalWrite(relb, HIGH);
  digitalWrite(rela, LOW);
  digitalWrite(relc, LOW);
}
