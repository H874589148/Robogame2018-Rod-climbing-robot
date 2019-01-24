#include "stm32f10x.h"
#include "delay.h"
	
  u32 m,black=0;
 void FUN(float dis){
	 dis=0.17*m;}
	 
void EXTI1_IRQHandler(void)							   //  ��1�ⲿ�жϵ��ж���Ӧ����
{	 
	   EXTI_ClearITPendingBit(EXTI_Line1);
	   if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1))		    TIM2->CNT =0;
		 else        m=TIM2->CNT ;
		 
}
void TIM2_IRQHandler(void)
{
	 TIM_ClearITPendingBit(TIM2,TIM_IT_Update);	
	 black=GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
}
void DIS_INT(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;   //�����ʽΪGPIO_InitTypeDef�Ľṹ�������ΪGPIO_InitStructure
	EXTI_InitTypeDef EXTI_InitStructure;				   //����һ�жϱ���
	 NVIC_InitTypeDef NVIC_InitStructure;		 //	  ����һ���ȼ�����
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	  //�����ʽΪTIM_TimeBaseInitTypeDef�Ľṹ�������ΪTIM_TimeBaseStructure  
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	  //ʹ��APB2���������ʱ�ӣ�����GPIOC, ���ܸ���ʱ��AFIO�ȣ�
                                                                              //�������������裬����̼����ֲ������
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
                



  
	    
                                       	  //typedef struct { u16 GPIO_Pin; GPIOSpeed_TypeDef GPIO_Speed; GPIOMode_TypeDef GPIO_Mode; } GPIO_InitTypeDef;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 //����IO�ڵĹ���ģʽ�������(io���ڲ���ӵ��赽��Դ��
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����IO����ߵ��������Ϊ50M
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;  //���ñ�ѡ�еĹܽţ�|��ʾͬʱ��ѡ��
    GPIO_Init(GPIOA, &GPIO_InitStructure);			        //��ʼ��GPIOB����ӦIO��Ϊ�������ã����ڰ�����⣬��demoͨ���ⲿ�жϕ�ʽʵ��
   

	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 //����IO�ڵĹ���ģʽΪ�������루��io���ڲ���ӵ��赽��Դ��
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����IO����ߵ��������Ϊ50M
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  //���ñ�ѡ�еĹܽţ�|��ʾͬʱ��ѡ��
    GPIO_Init(GPIOA, &GPIO_InitStructure);			        //��ʼ��GPIOC����ӦIO��Ϊ�������ã����ڰ�����⣬��demoͨ���ⲿ�жϕ�ʽʵ��
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);  // ѡ��GPIOB��Pin1��Ϊ�ⲿ�ж�����Դ


GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 //����IO�ڵĹ���ģʽΪ�������루��io���ڲ���ӵ��赽��Դ��
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //����IO����ߵ��������Ϊ50M
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  //���ñ�ѡ�еĹܽţ�|��ʾͬʱ��ѡ��
    GPIO_Init(GPIOA, &GPIO_InitStructure);			        //��ʼ��GPIOC����ӦIO��Ϊ�������ã����ڰ�����⣬��demoͨ���ⲿ�жϕ�ʽʵ��


  

	EXTI_InitStructure.EXTI_Line = EXTI_Line1;		//�����ж���Ϊ1
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;        //����Ϊ�ж�ģʽ
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;     //����Ϊ�������½��ش���
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                  //�����ж�Ϊʹ��
  EXTI_Init(&EXTI_InitStructure);                            //��ʼ��
	


 
    

		 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 	 //	   �����ȼ����镽ʽ����Ϊgroup1,��2����ռ����ϣ����ȼ���8����Ӧ���ȼ�
     NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;  //	���ж�Ϊ1�����ⲿ�ж�
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //������ȼ�Ϊ1���ڸ�����Ϊ�ϵ͵ģ�0���ȼ����
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			// ��Ӧ���ȼ�0��������ȼ�һ��ʱ�������
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//	����ʹ��
     NVIC_Init(&NVIC_InitStructure);							//	��ʼ��

     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);    //	   �����ȼ����镽ʽ����Ϊgroup1,��2����ռ����ϣ����ȼ���8����Ӧ���ȼ�
     NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; //���ж�ΪTIM4��������ж�
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//������ȼ�Ϊ1���ڸ�����Ϊ�ϵ͵ģ�0���ȼ����
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1 ;// ��Ӧ���ȼ�0��������ȼ�һ��ʱ��0���
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	 	//	����ʹ��
     NVIC_Init(&NVIC_InitStructure);					   	//	��ʼ��
 	   


  	

  	TIM_TimeBaseStructure.TIM_Period =65535;		  // ���ü�����ֵ
		TIM_TimeBaseStructure.TIM_Prescaler =71;		 //	 1us
  	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	// ʱ�ӕ�Ƶ����
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	// ������ʽΪ���ϼ���
	  
	  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);		 //	 ��ʼ��tim4
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update); //���TIM4����жϱ�־
    TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);   //  ʧ��TIM4����������ж�
    TIM_Cmd(TIM2,ENABLE);				 //		   ʹ��TIM4

	}

	