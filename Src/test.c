#include "test.h"
#include "leddrv.h"
#include "string.h"

uint16_t accStep[ACC_STEP_NUM] ;
ULTRASONIC myUltrasonic;

uint8_t testD[] = {"152"};

void ledTest(void){
    uint8_t testA[] = {"*Cairne Studio*"};
    uint8_t testB[] = {"UltrasonicModule"};
    uint8_t testC[] = {"Distance is:"};
    Lcd_Clear(GRAY0);
    Gui_DrawFont_GBK16(0, 16, GRAY2, WHITE,(uint8_t*) &testA);
    Gui_DrawFont_GBK16(0,40,GRAY1,GRAY0,(uint8_t*)&testB);
    Gui_DrawFont_GBK16(0,64,GRAY2,GRAY0,(uint8_t*)&testC);
    LCD_BLK_SET;
}

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
    mySensor->distance = 0;
    mySensor->num = 1;
}


void reverseCString(uint8_t* ch){
    uint8_t size = strlen(ch);
    uint8_t head = 0;
    uint8_t tail = size - 1;
    while(head < tail){
        uint8_t temp = ch[head];
        ch[head++] = ch[tail];
        ch[tail--] = temp;
    }
}
void itoa(uint32_t n,uint8_t s[]){
    int i,  sign;
    // uint8_t s[] = {};
    if((sign = n) < 0){
        n = -n;
    }
    i = 0;
    do{
        s[i++] = n%10 + '0';
    }while((n /= 10) > 0);
    if(sign < 0){
        s[i++] = '-';
    }
    s[i] = '\0';
    // reverseCString(&s);
    // sout = s;
}