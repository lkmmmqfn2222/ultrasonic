#ifndef _TEST_H
#define _TEST_H
#include "main.h"
#include "tim.h"

#define ACC_STEP_NUM 16

typedef struct {
    uint8_t num;            //number of ultrasonic sensors
    uint8_t sensorType;     //0:一体式 1：分立式
    uint8_t trigFlag;       //sensor drivered flag 0:no trig 1:trig
    uint8_t trigEndFlag;
    uint32_t timCnt0;       //
    uint32_t timCnt1;       //
    float   distance;       //
    float   distanceTemp;   //
    float preDistance;      //
}ULTRASONIC,*pULTRASONIC;

void ledTest(void);
void pwmTest(void);
void itoa(uint32_t n,uint8_t s[]);
void reverseCString(uint8_t* ch);
#endif