#include "stm32f10x.h"
u8  counter=0; 
u8  pwm=126;
int flag=0,a=0,b=0,period=50,n=0,i=0,m,p;
void TIM4_IRQHandler(void)	{	//	  //TIM3����������ж���Ӧ����������pwm��
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);	//	 //	 ���TIM3����ж���Ӧ������־λ
	
		if(counter==50)            //counter ��0��255�ۼ�ѭ��������ÿ��һ���жϣ�counter��һ
		{n+=1;
		counter =0;}
		else
		counter+=1;
		if(counter <=5)              //��counterֵС��pwmֵʱ����IO����Ϊ�ߣ���counterֵ���ڵ���pwmʱ����IO���õ�
		{GPIO_SetBits(GPIOA,GPIO_Pin_1);}//	��PC14 PC15��Ϊ�͵�ƽ
    else 
		{GPIO_ResetBits(GPIOA,GPIO_Pin_1);}//	��PC14 PC15��Ϊ�͵�ƽ*/
		
		
		
	
	}
void TIM_DIS_Init(){
	
	//������йس�ʼ��
	//��tim4
	/*********
	PA1  �����ƶ�  ��Trig
	PA2  �����ն�  ��Echo
	************/
	
	GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;		
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource2);  // ѡ��GPIOA��Pin2��Ϊ�ⲿ�ж�����Դ
	
	TIM_TimeBaseStructure.TIM_Period = 9; 	 //10us
	TIM_TimeBaseStructure.TIM_Prescaler = 71; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	
	 TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);		 //	 ��ʼ��tim4
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update); //���TIM4����жϱ�־
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);   //  ʹ��TIM4����������ж�
    TIM_Cmd(TIM4,ENABLE);				 //		   ʹ��TIM4
	
//	
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;		//�����ж���·Ϊ12��13
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;        //����Ϊ�ж�ģʽ
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;     //����Ϊ�����ش����ж�
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                  //�����ж�Ϊʹ��
  EXTI_Init(&EXTI_InitStructure);                            //��ʼ��
}



void nvic()								   //�����ж����ȼ�
{	 
     NVIC_InitTypeDef NVIC_InitStructure;		 //	  ����һ���ȼ�����


		 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 	 //	   �����ȼ����鷽ʽ����Ϊgroup1,��2����ռ����ϣ����ȼ���8����Ӧ���ȼ�
     NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;  //	���ж�Ϊ10-15��·���ⲿ�ж�
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //������ȼ�Ϊ1���ڸ�����Ϊ�ϵ͵ģ�0���ȼ����
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			// ��Ӧ���ȼ�0��������ȼ�һ��ʱ�������
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//	����ʹ��
     NVIC_Init(&NVIC_InitStructure);							//	��ʼ��
	  
 	   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);    //	   �����ȼ����鷽ʽ����Ϊgroup1,��2����ռ����ϣ����ȼ���8����Ӧ���ȼ�
     NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; //���ж�ΪTIM4��������ж�
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//������ȼ�Ϊ1���ڸ�����Ϊ�ϵ͵ģ�0���ȼ����
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // ��Ӧ���ȼ�0��������ȼ�һ��ʱ��0���
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	 	//	����ʹ��
     NVIC_Init(&NVIC_InitStructure);					   	//	��ʼ��

	 
 	   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //Ҫ��ͬһ��Group
     NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //TIM3	��������ж�
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//	������ȼ�Ϊ1������һ����ͬ����ϣ���ж��໥��϶Է�
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 	//	��Ӧ���ȼ�1��������һ�����������ж�ͬʱ��ʱ����һ����ִ��
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);

 
}
