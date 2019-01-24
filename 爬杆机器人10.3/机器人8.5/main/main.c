//STLINK PA13 PA14
//RFID ��ʼ���ɹ�����UID��BLOCKʧ��(λ�����⣩  ��ҪPA9 PA10
//��������ֱ۳ɹ�����  PA7 PA8  ��ת�Ķ����һ������PA11���ƣ�360��,���񲻴�һ����
//���ɹ��������ݴ���distance.c��m�У�0.17*m��Ϊ���룬�迼����ν�mͬ����main.c��PA1,PA2 һ��ģ����Ҫ����IO�� ��Ҫ������������Ʋ�����ʱ��������
//�����ת PB6-9 67����1������89����2����ת��V��ȡ0-99 �����ٶ�
//PA0���ڰף�д����Distance�� ��Ϊ��
#include "stm32f10x.h"
#include "math.h"
#include "stdio.h"
#include "delay.h"
#include "SteeringEngine.h"
#include "Distance.h"
#include "sys.h"
#include "usart.h"
#include "rfid.h"
//ALIENTEKս��STM32������ʵ��4
//����ʵ��  
//����֧�֣�www.openedv.com
//������������ӿƼ����޹�˾ 
void RCC_Configuration(void);	 //ʱ�ӳ�ʼ������������ʱ��
void GPIO_Configuration(void);	 //IO�ڳ�ʼ���������书��
void delay_nus(u32);           //72Mʱ���£�Լ��ʱus
void delay_nms(u32);            //72Mʱ���£�Լ��ʱms
void pagan_init(void);
void duoji360(void);
void duoji_open(u16 num);
void duoji_close(u16 num);
void Wheel_up(void);


void assert_failed(uint8_t* file, uint32_t line)
{
 printf("Wrong parameters value: file %s on line %d\r\n", file, line);
 while(1);
}

void RCC_GPIO_Configuration(void){
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ��PG�˿�ʱ��

	/*GPIOA*/
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	 //���ñ�ѡ�еĹܽţ�|��ʾͬʱ��ѡ��
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	   //����IO�ڹ���ģʽΪ	��������
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   //����IO����ߵ��������Ϊ50M
  //GPIO_Init(GPIOA, &GPIO_InitStructure);		  //��ʼ��GPIOC����ӦIO��Ϊ�������ã����ڼ��ǰ���ĸ�����
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_11; //TIM3 CH2  TIM1 CH1~CH4�������Ƭ��TX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	
	/*GPIOB*/
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6| GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //TIM3 CH2  TIM1 CH1~CH4
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*GPIOG*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //�̵���-->PG.13 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOG, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOG.13
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 //����IO�ڵĹ���ģʽΪ�������루��io���ڲ���ӵ��赽��Դ��
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����IO����ߵ��������Ϊ50M
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_11;  //���ñ�ѡ�еĹܽţ�|��ʾͬʱ��ѡ��
  GPIO_Init(GPIOG, &GPIO_InitStructure);			        //��ʼ��GPIOC����ӦIO��Ϊ�������ã����ڰ�����⣬�;
}
int main(void)
{
	//u8 len;	
	//u16 times=0;
	//u8 data[100];
	//u8 uid[8]={0};
	//u8 block[4]={0};	  
	//NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	//uart_init(19200);	 //����1��ʼ��Ϊ19200
	//uart_send_mydata(uid,8);
	//DIS_INT();
	RCC_GPIO_Configuration();
	TIM_PWM_Init();
	setAngle(1,90);
	while(1)
	{ 
		/*t=RFID_Init();
		delay_nms(1000);
		tt=GET_UID(uid);//��ȡUID
		delay_nms(1000);
		ttt=RFID_READ(uid,block);//��ȡrfid��һ������
		delay_nms(1000);
		tttt=RFID_Disableantenna();//�ر����ߣ����ٷ��� 
		delay_nms(1000);*/
		;
	
 }
  
}
void pagan_init(void){
		GPIO_SetBits(GPIOG,GPIO_Pin_13);//������
}	
void duoji_open(u16 num){
	setAngle(num,0);
	delay_nms(2000);
}
void duoji_close(u16 num){
	setAngle(num,170);
	delay_nms(2000);
}
void duoji360(void){
  setAngle(2 ,110);
	delay_nms(2600);
	setAngle(2,90);
}
void Wheel_up(void){
	setWheel(1,1,20);
	delay_nus(2000);
	setWheel(2,1,20);
}