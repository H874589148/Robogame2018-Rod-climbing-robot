#include "stm32f10x.h"
u8 zhang=0,xue=0;
void TIM_PWM_Init(){
	
	//控制舵机的定时器PWM波输出初始化 
	
	/*********
	标*的为使用到的5个引脚
	TIM1-CH1 PA8  	*  2
	     CH4 PA11		*  3
	TIM3  
	     CH2 PA7		*  1
	     CH3 PB0    *  4
	     CH4 PB1    *  5
	TIM4-CH1 PB6     1正转
	     CH2 PB7
	     CH3 PB8      2正转
	     CH4 PB9
	************/
	
	
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);// 
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	
	
	
	TIM_TimeBaseStructure.TIM_Period = 1999; 	 //50HZ 20ms
	TIM_TimeBaseStructure.TIM_Prescaler = 719; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_TimeBaseStructure.TIM_Period = 99; 	 //
	TIM_TimeBaseStructure.TIM_Prescaler = 71; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
  TIM_OCInitStructure.TIM_Pulse = 0; 
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); 
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);       
  TIM_ARRPreloadConfig(TIM1, ENABLE); 
  TIM_Cmd(TIM1, ENABLE);  
	 
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); 
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); 
  TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable); 
  
	TIM_ARRPreloadConfig(TIM3, ENABLE); 
  
	TIM_Cmd(TIM3, ENABLE); 
xue=1;
	TIM_OC1Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
		TIM_OC3Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
  TIM_ARRPreloadConfig(TIM4, ENABLE); 
  TIM_Cmd(TIM4, ENABLE); 
//	
}

void setAngle(u16 ID ,u16 angle){
	
	//angle:0~180，适用于180度舵机
	
	u16 pwm = 50 + angle*10/9;   //pwm值50-250，脉冲宽度0.5-2.5ms，对应角度0~180度，270度舵机则对应0~270度
	
	switch(ID){
		case 1: //PA7
			TIM_SetCompare2(TIM3,pwm);
	    break;
		case 2: //PA8
			TIM_SetCompare1(TIM1,pwm);
      break;
		case 3: //PA11
			TIM_SetCompare4(TIM1,pwm);
		  break;
		case 4:
			TIM_SetCompare3(TIM3,pwm);
		  break;
		case 5:
			TIM_SetCompare4(TIM3,pwm);
		  break;
	}
	
	
}
void setWheel(u16 num, u16 dire,u16 v){
	if(num==1)
	{if(dire==1)
		{zhang=1;
		TIM_SetCompare1(TIM4,v);}
		else
		TIM_SetCompare2(TIM4,v);}
else 
{if(dire==1)
		TIM_SetCompare3(TIM4,v);
		else
		TIM_SetCompare4(TIM4,v);}	}
		
	
/*void nvic(){
	
	//定时器中断优先级配置
	//输出pwm貌似不用这个函数
	
		 NVIC_InitTypeDef NVIC_InitStructure;  //	 //	  命名一优先级变量

 	   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);    //	   将优先级分组方式配置为group1,有2个抢占（打断）优先级，8个响应优先级
     NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; //该中断为TIM2溢出更新中断
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//打断优先级为1，在该组中为较低的，0优先级最高
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; // 响应优先级0，打断优先级一样时，0最高
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	 	//	设置使能
     NVIC_Init(&NVIC_InitStructure);					   	//	初始化

	 
 	   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //要用同一个Group
     NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn; //TIM1	溢出更新中断
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//	打断优先级为1，与上一个相同，不希望中断相互打断对方
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 	//	响应优先级1，低于上一个，当两个中断同时来时，上一个先执行
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);
	
	   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //要用同一个Group
     NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //TIM3	溢出更新中断
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//	打断优先级为1，与上一个相同，不希望中断相互打断对方
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 	//	响应优先级1，低于上一个，当两个中断同时来时，上一个先执行
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);
	
}*/