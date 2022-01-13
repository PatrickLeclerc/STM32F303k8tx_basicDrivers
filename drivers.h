#include "stm32f303x8.h"
#include <string.h>

//System basics
void startup(void);
void initClock(void);
void msDelay(uint16_t delay);
void usDelay(uint16_t delay);
void initUSART2(void);
void printU2(char* data);
//Timers
void initTIM17(void);
void initTIM(uint32_t tim, uint32_t psc, uint32_t arr, uint8_t irqUsed);
//SPI
void initSPI(void);
