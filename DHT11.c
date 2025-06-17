#include "stm32f10x.h"                  // Device header
#include "Delay.h"

/*
device��STM32F103C8T6 ���� DHT11
���ã�DHT11�Ĵ���ͨ��
*/


#define A_PIN GPIO_Pin_10     //����ΪA pin10 ��

void DHT11_Init(void)	  //DHT11��GPIO��ʼ��
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = A_PIN;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_WriteBit(GPIOA,A_PIN,(BitAction)1);    //�� 1 ���߷ǿ���״̬

}
/*
����������ͨ�ſ�ʼ����
���ã���ʼDHT11�Ĵ���ͨ�ţ���������ǰ��׼��
*/

u8 DHT11_Start(void) 
{
    u32 timeout = 10000;  // ��ʼ��timeout��Լ10ms��ʱ
    
	GPIO_InitTypeDef GPIO_InitStructure;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;   //���� gpio��Ϊ�����©ģʽ�������ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = A_PIN;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_WriteBit(GPIOA,A_PIN,(BitAction)1);
	
    // 1. �����������ߡ�18ms
    GPIO_WriteBit(GPIOA, A_PIN, (BitAction)0);
    Delay_ms(20);  // ʵ����ʱ20msȷ���㹻
    
    // 2. �����ͷ�����
    GPIO_WriteBit(GPIOA, A_PIN, (BitAction)1);
    Delay_us(40);  // �ӳ���40��s
    
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //��������ģʽ��������������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = A_PIN;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
    // 3. �ȴ�DHT11����(Ӧ���ź�)
    while(GPIO_ReadInputDataBit(GPIOA, A_PIN) == 0)
    {
        if(--timeout == 0) return 0;  // ��ʱ���ش���
    }
    
    // 4. �ȴ�DHT11����
    timeout = 10000;  // ���ó�ʱ������
    while(GPIO_ReadInputDataBit(GPIOA, A_PIN) == 1)
    {
        if(--timeout == 0) return 0;  // ��ʱ���ش���
    }
    
    return 1;  // �ɹ��յ�Ӧ��

}
/*
������DHT11_RD_Bit����
���ã���ȡA_PIN�� 1bit ����

*/
uint8_t DHT11_RD_Bit(void)   
{
	return GPIO_ReadInputDataBit(GPIOA,A_PIN);
}
/*
������DHT11_RD_8bits����
���ã���ȡ���������8��bit��Ϣ

*/
u8 DHT11_RD_8bits(void)
{	
	u8 i,RData=0;
	u32 timeout =1000;
	
	for (i=0;i<=7;i++)
	{	
		while(DHT11_RD_Bit()==0); //���� 1 bit ��ʼ�׶� 50us����
		
		Delay_us(30);       //����һ����ʱ��30us�����ж��Ƿ�Ϊһ���Ϊ1���ʾ1��֮Ϊ0
		RData <<=1;    //����һ��λ�����и�ֵ����ֵ8��
		if(DHT11_RD_Bit()==1)  //�ж�PA10�Ƿ�Ϊ1���Ϊ1��ֵ1��RData��֮��ֵ0
		{
			RData |= 1;
			timeout =1000;
			while(GPIO_ReadInputDataBit(GPIOA,A_PIN)==1); //�ȴ���ʾ 1 ���źŽ���
			
		}
	}
	return RData;
}
/*
������u8 DHT11_RD_40bits(u8 *temp , u8 *humi)
@temp:���ز�����¶�
@humi�����ز����ʪ��
@u8 DHT11_RD_40bits(u8 *temp , u8 *humi):��������żУ��ĵó��Ľ����1Ϊ��ȷ��0Ϊ����
���ã���ȡ���ڴ���õ���40bit���ݣ�8bitʪ����������+8bitʪ��С������+8bi�¶���������+8bit�¶�С�����ݣ�

ע��������ֻ����¶Ⱥ�ʪ�ȵ���ֵ

*/
u8 DHT11_RD_40bits(u8 *temp , u8 *humi)
{
	u8 Data[5];    //����DATA��5��u8���͵���ֵ�ɴ洢
	u8 i;          
	
	if (DHT11_Start()==1)        //����ֵ���Ž�DATA��
	{
		for (i=0;i<5;i++)
		{
			Data[i]=DHT11_RD_8bits();         
		}
	
	}
	else{return 0;}
	if (Data[0]+Data[1]+Data[2]+Data[3]==Data[4])  //�ж���żУ���Ƿ���ȷ�ٽ��и�ֵ
	{
		*temp = Data[0];
		*humi = Data[2];
		return 1;
	}
	else return 0;
}
