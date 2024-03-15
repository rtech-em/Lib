#include "RTECHENLIB.h"

#include "Arduino.h"
#include <string>

RTECHEN::RTECHEN()
{
}
void RTECHEN::begin()
{
    Serial.println();
    Serial.println("-----> R-TECH EMBEDDED START LIBRARY <-----");
}

void RTECHEN::encryption(char *inputX)
{
  uint8_t ctr = 0;
  uint8_t codeX, codeRef;
  int data[12];
  char hideCode;
  for(int i = 0; i < 17; i++){
    if(i == 2 || i == 5 || i == 8 || i == 11 || i == 14){
      //Serial.println("Tidak Sama");
    }else{
      codeX = inputX[i];
      codeRef = _ref[ctr];
      data[ctr] = inputX[i] + _ref[ctr];
      if(data[ctr] > 126){
        data[ctr] = inputX[i] + ctr;
        hideCode = data[ctr];
      }else{
        hideCode = inputX[i] + _ref[ctr];
      }

     // _result[ctr] = hideCode;// Bug Error
      _myResult = _myResult + hideCode;
      // Serial.print(ctr);Serial.print(" => ");
      // Serial.print(codeX);Serial.print(" + ");
      // Serial.print(codeRef); Serial.print(" = "); Serial.print(data[ctr]); 
      // Serial.print(" => "); Serial.println(hideCode);
       ctr++;
    }
    
  }

  //Serial.print("HjhVb974BT "); 
  //Serial.print("Enc => ");
  //Serial.println(_result);
  //Serial.println(_myResult);
  //Serial.println(strlen(_result));
  ///Serial.println(" DhC1PQhfd0");
  
}

void RTECHEN::decryption(char *inputX)
{
  uint8_t tmp;
  char showCode;
  //Serial.print("Show : ");
  for(int i = 0; i < 12; i++){
    tmp = inputX[i] - _ref[i];
    if(i == 2 || i == 4 || i == 6 || i == 8 || i == 10){
      Serial.print(":");
    }
    showCode = tmp;
    //Serial.print(showCode);
  }
}

uint8_t RTECHEN::checking(char *inputX, char *inputY)
{
  // memset(_result, 0, sizeof(_result));
  // encryption(inputX); 
  // Serial.println(_result);
  // Serial.println(inputY);
  // Serial.println(strcmp(_result, inputY));
  // //if(_result == inputY){
  // if(strcmp(_result, inputY) == 0){
  //   return 0;
  // }else
  // {
  //   return 1;
  // }
  
}

uint8_t RTECHEN::getEnc(char *inputX, char *inputY)
{
  String inpY;
  uint8_t val;
  encryption(inputX); 
  inpY = inputY;
  if(_myResult == inpY){
      Serial.println("Eq");
      val = 0;
  }else{
    Serial.println("N-Eq");
    val = 1;
  }
  return val;
  
}

void RTECHEN::printEnc()
{
  Serial.println(_myResult);
}
