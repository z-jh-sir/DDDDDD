#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "DHT11.h"

//≤‚ ‘

u8 temp,humi;
u8 check;

int main(void)
{
	OLED_Init();
	DHT11_Init();
	
	OLED_ShowString(1,1,"Check:");
	OLED_ShowString(2,1,"temp:");
	OLED_ShowString(3,1,"humi:");
	
	
	
	while(1)
	{
		
		check = DHT11_RD_40bits(&humi,&temp);

		OLED_ShowBinNum(1,7,(u32)check,1);
		OLED_ShowNum(2,6,(u32)temp,3);
		OLED_ShowNum(3,6,(u32)humi,3);
	
	}
}
