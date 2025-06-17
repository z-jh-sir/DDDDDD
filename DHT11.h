#ifndef _DHT11_H
#define _DHT11_H

void DHT11_Init(void);    //DHT11的GPIO初始化 
u8 DHT11_Start(void);  //开始DHT11的串口通信，输入数据前的准备
uint8_t DHT11_RD_Bit(void);  //读取A_PIN的 1bit 输入
u8 DHT11_RD_8bits(void);   //读取A_PIN的 8bit 输入
u8 DHT11_RD_40bits(u8 *temp , u8 *humi);  //读取A_PIN的 40bit 输入

#endif

