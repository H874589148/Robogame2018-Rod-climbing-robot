#include "stm32f10x.h"

void stepper_Init(u16 divNum, double round){
	
	//divNum：细分数，改变细分数也要电机上调节电流，目前取16
	//步进电机 每秒5圈已经算快了
	//转速较快时，务必逐渐加速，如3-5-6，比如不能直接从0到5
	//round: 每秒的圈数，可能需要改变电流
	
	/*******
	接线说明：
	PA0-PUL+
	PA1-DIR+
	PUL-、DIR-、GND与单片机公共地
	驱动Vcc接电源：9-12V即可
	********/
	
	u32	pulseNum = (u32)divNum*200*round;
  u32 arr = 1000000/pulseNum;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);		//开启时钟 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_2; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);										//初始化IO口 

	TIM_TimeBaseStructure.TIM_Period = arr-1;	 
	TIM_TimeBaseStructure.TIM_Prescaler =72-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);								//初始化定时器 

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;					//设置定时器工作模式 

	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);

	TIM_ARRPreloadConfig(TIM2, ENABLE); 
	
	TIM_Cmd(TIM2, ENABLE);
	
	TIM_SetCompare1(TIM2,5);						//脉宽5us 
}

void stepper_direction(u8 direction){
	
	//方向控制 
	//不能在转速较快时反向
	
	if(direction == 0)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	}
}

void stepper_enable(u8 status){
	
	//控制是否转
	//0 不转, 1 转
	
	if(status == 0)
	{
		TIM_Cmd(TIM2, DISABLE);
		
	}
	else
	{
		TIM_Cmd(TIM2, ENABLE);
	}
}
