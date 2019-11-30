#include "test.h"

uint16_t accStep[ACC_STEP_NUM] ;
ULTRASONIC myUltrasonic;

void pwmTest(void){
    myUltrasonic.trigEndFlag = 0;
    myUltrasonic.trigFlag = 0;  
    for(int i = 1; i < ACC_STEP_NUM ; i++){
        accStep[i] = accStep[i-1] + 11;
    }
    HAL_TIM_OC_Start_DMA(&htim1,TIM_CHANNEL_2,(uint32_t*)accStep,ACC_STEP_NUM);
}


// void ultralsonicTest(pULTRASONIC mySensor){

// }

void ultrasonicInit(pULTRASONIC mySensor){
    
}