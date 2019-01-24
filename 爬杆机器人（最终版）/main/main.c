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
u8 RxData=0x00;
u8 flag=0;
u8 flag_count=0;
u8 f1=1,f2=1;
void RCC_Configuration(void){	 //ʱ�ӳ�ʼ������������ʱ
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG , ENABLE);//
}
void GPIO_Configuration(void){	 //IO�ڳ�ʼ���������书��
  GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_11; //TIM3 CH2  TIM1 CH1~CH4
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_5|GPIO_Pin_6| GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10; //TIM3 CH2  TIM1 CH1~CH4
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
//	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
//	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
//	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
}
void USART3_RCC_Configuration(void){
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
 //  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   //��GPIOA,AFIO,USART1����ʱ��
  
}
void nvic(void){
     NVIC_InitTypeDef NVIC_InitStructure;  //	 //	  ����һ���ȼ�����

 	   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);    //	   �����ȼ����鷽ʽ����Ϊgroup1,��2����ռ����ϣ����ȼ���8����Ӧ���ȼ�
     NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; //���ж�ΪTIM2��������ж�
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//������ȼ�Ϊ1���ڸ�����Ϊ�ϵ͵ģ�0���ȼ����
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // ��Ӧ���ȼ�0��������ȼ�һ��ʱ��0���
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	 	//	����ʹ��
     NVIC_Init(&NVIC_InitStructure);					   	//	��ʼ��

	 	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);    //	   �����ȼ����鷽ʽ����Ϊgroup1,��2����ռ����ϣ����ȼ���8����Ӧ���ȼ�
     NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //���ж�ΪTIM2��������ж�
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//������ȼ�Ϊ1���ڸ�����Ϊ�ϵ͵ģ�0���ȼ����
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; // ��Ӧ���ȼ�0��������ȼ�һ��ʱ��0���
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	 	//	����ʹ��
     NVIC_Init(&NVIC_InitStructure);					   	//	��ʼ�� 
	
 	   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //Ҫ��ͬһ��Group
     NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn; //TIM1	��������ж�
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//	������ȼ�Ϊ1������һ����ͬ����ϣ���ж��໥��϶Է�
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 	//	��Ӧ���ȼ�1��������һ�����������ж�ͬʱ��ʱ����һ����ִ��
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);
	
	   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //Ҫ��ͬһ��Group
     NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; //TIM3	��������ж�
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//	������ȼ�Ϊ1������һ����ͬ����ϣ���ж��໥��϶Է�
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 	//	��Ӧ���ȼ�1��������һ�����������ж�ͬʱ��ʱ����һ����ִ��
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);
}
void USART3_Configuration(void){
	  USART_InitTypeDef USART_InitStructure; //�������ûָ�Ĭ�ϲ���
  
	  USART3_RCC_Configuration();
	  nvic();
	  
	  USART_InitStructure.USART_BaudRate = 38400; //������9600
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�ֳ�8λ
    USART_InitStructure.USART_StopBits = USART_StopBits_1; //1λֹͣ�ֽ�
    USART_InitStructure.USART_Parity = USART_Parity_No; //����żУ��
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;     //��������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;      //��Rx���պ�Tx���͹���
    USART_Init(USART3, &USART_InitStructure);  //��ʼ��
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  // ���������ݼĴ�������������ж�
    USART_Cmd(USART3, ENABLE);                     //��������                   
                                                   
       //-----�����������1���ֽ��޷���ȷ���ͳ�ȥ������-----// 
    USART_ClearFlag(USART3, USART_FLAG_TC);     // �巢����ɱ�־
}
void delay_nus(u32);           //72Mʱ���£�Լ��ʱus
void delay_nms(u32);            //72Mʱ���£�Լ��ʱms
void pagan_init(void);
void duoji_1(void);
void duoji_2(void);
void duoji_3(void);
void hand_open(u16 num);
void hand_close(u16 num);
void arm_open(u16 num);
void arm_close(u16 num);
void put_1(void);
void put_2(void);
void Wheel_up(void);
void STOP(void);
void assert_failed(uint8_t* file, uint32_t line)
{
 printf("Wrong parameters value: file %s on line %d\r\n", file, line);
 while(1);
}

int main(void)
{
	RCC_Configuration();
	GPIO_Configuration();
	TIM_PWM_Init();
  duoji_1();
	delay_nus(20);
	hand_open(1);
	arm_close(4);
	delay_nus(20);
	hand_open(3);
	arm_close(5);
	delay_nus(20);
	USART3_Configuration();
	//Wheel_up();
	while(1)
	{ 
		if(flag==1){
			f1=GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_12);
			//f2=GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_12);
			if(f1==0&&f2==0){
				delay_nus(20);
				f1=GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_12);
				if(f1==0){
				duoji_3();
				arm_open(4);
				arm_open(5);
				hand_open(1);
				hand_open(3);
				STOP();//�¸˲�����ͣ�����
				}
			}
		}
	
 }
  
}
void pagan_init(void){
		setWheel(2,2,100);
	  delay_nms(1000);
	  setWheel(1,1,100);
	  setWheel(2,1,90);
    flag=1;
}	
void hand_open(u16 num){
	switch(num){
		case 1:
	    setAngle(num,90);
		  break;
		case 3:
			setAngle(num,110);
		  break;
	}
	delay_nms(2000);
}
void hand_close(u16 num){
	switch(num){
		case 1:
	    setAngle(num,140);
		  break;
		case 3:
			setAngle(num,170);
		  break;
	}
	delay_nms(2000);
}
void arm_open(u16 num){
	switch(num){
		case 4:
	    setAngle(num,140);
		  break;
		case 5:
			setAngle(num,150);
		  break;
	}
	delay_nms(2000);
}
void arm_close(u16 num){
	switch(num){
		case 4:
	    setAngle(num,46);
		  break;
		case 5:
			setAngle(num,54);
		  break;
	}
	delay_nms(2000);
}
void arm_half(u16 num){
	switch(num){
		case 4:
	    setAngle(num,83);
		  break;
		case 5:
			setAngle(num,101);
		  break;
	}
	delay_nms(2000);
}
void duoji_1(void){
  setAngle(2 ,5);
	delay_nms(2000);
}
void duoji_2(void){
	setAngle(2 ,180);
	delay_nms(2000);
}
void duoji_3(void){
	setAngle(2 ,90);
	delay_nms(2000);
}
void put_1(void){
	setAngle(2 ,60);
	delay_nms(2000);
}
void put_2(void){
	setAngle(2 ,135);
	delay_nms(2000);
}
void Wheel_up(void){
	setWheel(2,1,90);
	delay_nus(2000);
	setWheel(2,2,100);
}	
void STOP(void){
	setWheel(2,1,0);
	setWheel(2,2,0);
}
void USART3_IRQHandler(void){
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET) //�ж��Ƿ�Ϊ�����ж�
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);	//��������жϱ�־λ
		RxData=USART_ReceiveData(USART3);								//��ȡ��������{
			switch(RxData)
			{
				case (u8)0x61:
					USART_SendData(USART3, 'S');
				  break;
				case (u8)0x41: 
				  break;
				case (u8)0x42:
					if(flag_count==0)
						duoji_2();
				  USART_SendData(USART3, 'T');
				  break;
				case (u8)0x43: 
					arm_open(4);
					hand_close(1);
				  USART_SendData(USART3, 'T');
				  arm_half(4);
				  flag_count++;
				  if(flag_count==1)
						duoji_2();
				  break;
				case (u8)0x44: 
					pagan_init();
				  USART_SendData(USART3, 'T');
				  break;
				case (u8)0x45:
					arm_open(5);
				  delay_nms(1000);
					hand_close(3);
				  delay_nms(1000);
				  USART_SendData(USART3, 'T');
				  arm_half(5);
				  flag_count++;
				  duoji_1();
				  break;
				case (u8)0x46:
				  arm_open(4);
				  hand_close(1);
				  USART_SendData(USART3, 'T');
				  arm_half(4);
				  flag_count++;
				  if(flag_count==1)
						duoji_2();
				  break;
				case (u8)0x47:
					arm_open(5);
				  hand_close(3);
				  USART_SendData(USART3, 'T');
				  arm_half(5);
				  flag_count++;
						duoji_1();
				  break;
				case (u8)0x50:
					put_1();
				  delay_nms(2000);
				  arm_open(4);
				  hand_open(1);
				  arm_close(4);
				  put_2();
				  delay_nms(3000);
				  arm_open(5);
				  hand_open(3);
				  delay_nms(2000);
				  arm_close(5);
				  USART_SendData(USART3, 'T');
				  flag_count=0;
				  duoji_1();
				  break;
				case (u8)0x52:
          break;				
				default:break;
			}
		
	}
	
	
	if(USART_GetITStatus(USART3,USART_IT_TXE)!=RESET)  //�ж��Ƿ�Ϊ�����ж�
	{	
		USART_ClearITPendingBit(USART3,USART_IT_TXE);//��������жϱ�־λ
		
	}
	if(USART_GetITStatus(USART3,USART_IT_TC)!=RESET)  //�ж��Ƿ�Ϊ��������ж�
	{
		USART_ClearITPendingBit(USART3,USART_IT_TC);	//�����������жϱ�־λ
	}

 }	 