// 1. Enable clock access of GPIO PIN
// 2. Se the PINS Mode
// 3. set output

// AHB1_EN	
// green PD12
// orange PD13
// red PD14
// blue PD15

#include "stm32f4xx.h"                  // Device header


/*from data sheet GPIOD is connected to LED and corresponding bits are colors given*/
#define RED 		(1U<<14)     //set 14th bit
#define GREEN 	(1U<<12)		 //set 12th bit
#define BLUE 		(1U<<15)		 //set 15th bit
#define ORANGE 	(1U<<13)		 //set 15th bit

/* GPIOx_MODER port mode register ==> decides mode of regester 00,01,10,11 4 types possible 2 bit
is required to set, for to make output set pins to 01*/
#define RED_BIT 		(1U<<28)
#define GREEN_BIT 	(1U<<24)
#define BLUE_BIT 		(1U<<30)
#define ORANGE_BIT 	(1U<<26)

/* from data sheet AHB1 is connected to PORTD to enable clock for PORTD we should use 4th bit*/
#define GPIOD_CLOCK (1<<3)

volatile uint32_t tick;   //volatile:- compiler should know variable can change without any system code changing
volatile uint32_t _tick;
void GPIO_init(void);
uint32_t getTick(void);
void DelayS(uint32_t seconds);

int main(){
	GPIO_init();
	while(1){
		GPIOD->ODR |= RED|GREEN|BLUE|ORANGE; //ODR=> output data register
		DelayS(1);
	}
}

void GPIO_init(){
	RCC->AHB1ENR |= GPIOD_CLOCK;   // Reset and clock control
	GPIOD->MODER |= RED_BIT|GREEN_BIT|BLUE_BIT|ORANGE_BIT; // MODER port mode register
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/100U);
	__enable_irq();
}

void SysTick_Handler(void){
	++tick;    //SysTick_Hnalder need_eplanation available in futher studies
}

uint32_t getTick(void){
	__disable_irq();
	_tick = tick;      // dont want any interupt to interupt this process can cause race around condition
	__enable_irq();
	return _tick;
}

void DelayS(uint32_t seconds){
	seconds *=100;
	uint32_t temp = _tick;
	while(getTick()-temp < seconds){}
}





