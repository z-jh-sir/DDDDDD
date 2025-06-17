#include "stm32f10x.h"                  // Device header
#include "Delay.h"

/*
device：STM32F103C8T6 控制 DHT11
作用：DHT11的串口通信
*/


#define A_PIN GPIO_Pin_10     //设置为A pin10 脚

void DHT11_Init(void)	  //DHT11的GPIO初始化
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = A_PIN;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_WriteBit(GPIOA,A_PIN,(BitAction)1);    //置 1 总线非空闲状态

}
/*
函数：串口通信开始函数
作用：开始DHT11的串口通信，输入数据前的准备
*/

u8 DHT11_Start(void) 
{
    u32 timeout = 10000;  // 初始化timeout，约10ms超时
    
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;   //设置 gpio口为输出开漏模式进入输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = A_PIN;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_WriteBit(GPIOA,A_PIN,(BitAction)1);
	
    // 1. 主机拉低总线≥18ms
    GPIO_WriteBit(GPIOA, A_PIN, (BitAction)0);
    Delay_ms(20);  // 实际延时20ms确保足够
    
    // 2. 主机释放总线
    GPIO_WriteBit(GPIOA, A_PIN, (BitAction)1);
    Delay_us(40);  // 延长到40μs
    
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //进入输入模式，设置上拉输入
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = A_PIN;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    // 3. 等待DHT11拉低(应答信号)
    while(GPIO_ReadInputDataBit(GPIOA, A_PIN) == 0)
    {
        if(--timeout == 0) return 0;  // 超时返回错误
    }
    
    // 4. 等待DHT11拉高
    timeout = 10000;  // 重置超时计数器
    while(GPIO_ReadInputDataBit(GPIOA, A_PIN) == 1)
    {
        if(--timeout == 0) return 0;  // 超时返回错误
    }
    
    return 1;  // 成功收到应答

}
/*
函数：DHT11_RD_Bit（）
作用：读取A_PIN的 1bit 输入

*/
uint8_t DHT11_RD_Bit(void)   
{
	return GPIO_ReadInputDataBit(GPIOA,A_PIN);
}
/*
函数：DHT11_RD_8bits（）
作用：读取串口输入的8个bit信息

*/
u8 DHT11_RD_8bits(void)
{	
	u8 i,RData=0;
	u32 timeout =1000;
	
	for (i=0;i<=7;i++)
	{	
		while(DHT11_RD_Bit()==0); //经过 1 bit 开始阶段 50us结束
		
		Delay_us(30);       //进行一个延时到30us，再判断是否为一如果为1则表示1反之为0
		RData <<=1;    //左移一个位数进行赋值，赋值8次
		if(DHT11_RD_Bit()==1)  //判断PA10是否为1如果为1则赋值1给RData反之赋值0
		{
			RData |= 1;
			timeout =1000;
			while(GPIO_ReadInputDataBit(GPIOA,A_PIN)==1); //等待表示 1 的信号结束
			
		}
	}
	return RData;
}
/*
函数：u8 DHT11_RD_40bits(u8 *temp , u8 *humi)
@temp:返回测出的温度
@humi：返回测出的湿度
@u8 DHT11_RD_40bits(u8 *temp , u8 *humi):自身返回奇偶校验的得出的结果，1为正确，0为错误
作用：读取串口传输得到的40bit数据（8bit湿度整数数据+8bit湿度小数数据+8bi温度整数数据+8bit温度小数数据）

注：本函数只输出温度和湿度的数值

*/
u8 DHT11_RD_40bits(u8 *temp , u8 *humi)
{
	u8 Data[5];    //设置DATA有5个u8类型的数值可存储
	u8 i;          
	
	if (DHT11_Start()==1)        //将数值都放进DATA里
	{
		for (i=0;i<5;i++)
		{
			Data[i]=DHT11_RD_8bits();         
		}
	
	}
	else{return 0;}
	if (Data[0]+Data[1]+Data[2]+Data[3]==Data[4])  //判断奇偶校验是否正确再进行赋值
	{
		*temp = Data[0];
		*humi = Data[2];
		return 1;
	}
	else return 0;
}
