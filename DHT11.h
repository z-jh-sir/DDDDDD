#ifndef _DHT11_H
#define _DHT11_H

void DHT11_Init(void);    //DHT11��GPIO��ʼ�� 
u8 DHT11_Start(void);  //��ʼDHT11�Ĵ���ͨ�ţ���������ǰ��׼��
uint8_t DHT11_RD_Bit(void);  //��ȡA_PIN�� 1bit ����
u8 DHT11_RD_8bits(void);   //��ȡA_PIN�� 8bit ����
u8 DHT11_RD_40bits(u8 *temp , u8 *humi);  //��ȡA_PIN�� 40bit ����

#endif

