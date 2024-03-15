#include "RtechI2c.h"

/*
    Scan masing masing address i2c pada modul
*/
RTECHI2 modul_1(0x3F); //Ganti dengan address modul 1
RTECHI2 modul_2(0x3B); //Ganti dengan address modul 1

int m1_0 = 0;
int m1_1 = 1;
int m1_2 = 2;
int m1_3 = 3;
int m1_4 = 4;
int m1_5 = 5;

int m2_0 = 0;
int m2_1 = 1;
int m2_2 = 2;
int m2_3 = 3;
int m2_4 = 4;
int m2_5 = 5;

void setup() {
  modul_1.begin();
  modul_2.begin();
  
  modul_1.pinMode(m1_0, OUTPUT);
  modul_1.pinMode(m1_1, OUTPUT);
  modul_1.pinMode(m1_2, OUTPUT);
  modul_1.pinMode(m1_3, OUTPUT);
  modul_1.pinMode(m1_4, OUTPUT);
  modul_1.pinMode(m1_5, OUTPUT);

  modul_2.pinMode(m2_0, OUTPUT);
  modul_2.pinMode(m2_1, OUTPUT);
  modul_2.pinMode(m2_2, OUTPUT);
  modul_2.pinMode(m2_3, OUTPUT);
  modul_2.pinMode(m2_4, OUTPUT);
  modul_2.pinMode(m2_5, OUTPUT);


}

void loop() {
  modul_1.digitalWrite(m1_0, HIGH);
  modul_1.digitalWrite(m1_1, HIGH);
  modul_1.digitalWrite(m1_2, HIGH);
  modul_1.digitalWrite(m1_3, HIGH);
  modul_1.digitalWrite(m1_4, HIGH);
  modul_1.digitalWrite(m1_5, HIGH);

  modul_2.digitalWrite(m2_0, HIGH);
  modul_2.digitalWrite(m2_1, HIGH);
  modul_2.digitalWrite(m2_2, HIGH);
  modul_2.digitalWrite(m2_3, HIGH);
  modul_2.digitalWrite(m2_4, HIGH);
  modul_2.digitalWrite(m2_5, HIGH);

  delay(1000);

  modul_1.digitalWrite(m1_0, LOW);
  modul_1.digitalWrite(m1_1, LOW);
  modul_1.digitalWrite(m1_2, LOW);
  modul_1.digitalWrite(m1_3, LOW);
  modul_1.digitalWrite(m1_4, LOW);
  modul_1.digitalWrite(m1_5, LOW);

  modul_2.digitalWrite(m2_0, LOW);
  modul_2.digitalWrite(m2_1, LOW);
  modul_2.digitalWrite(m2_2, LOW);
  modul_2.digitalWrite(m2_3, LOW);
  modul_2.digitalWrite(m2_4, LOW);
  modul_2.digitalWrite(m2_5, LOW);

  delay(1000);

}