#ifndef RTECHENLIB_H
#define RTECHENLIB_H

#include "Arduino.h"
#include <string>

class RTECHEN{
    public:
        RTECHEN();
        void begin();
        void encryption(char *msg);
        void decryption(char *msg);
        uint8_t checking(char *inputX, char *inputY);
        uint8_t getEnc(char *msg, char *inputY);
        void printEnc();
    private:
        char _ref[12] = {'7','/','3','6','+','!','@','%','1','-','4','5'};
        char _result[12];
        String _myResult;


};

#endif;