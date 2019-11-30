#ifndef _MYPRINT_H
#define _MYPRINT_H

#include "main.h"
#include "stdio.h"
#include "stdarg.h"

UART_HandleTypeDef huart1;

void    myPrint(char* fmt, ...);
void    printCh(char ch);
void    printDec(int dec);
void    printFlt(double flt);
void    printBin(int bin);
void    printHex(int hex);
void    printStr(char* str);


#endif