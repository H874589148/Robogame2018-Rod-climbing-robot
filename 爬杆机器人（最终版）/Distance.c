#include "stm32f10x.h"
u8  counter=0; 
u8  pwm=126;
int flag=0,a=0,b=0,period=50,n=0,i=0,m,p;
void TIM4_IRQHandler(void)	{	//	  //TIM3的溢出更新中断响应函数，产生pwm波
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);	//	 //	 清空TIM3溢出中断响应函数标志位
	
		if(counter==50)            //counter 从0到255累加循环计数，每进一次中断，counter加一
		{n+=1;
		counter =0;}
		else
		counter+=1;
		if(counter <=5)              //当counter值小于pwm值时，将IO口设为高；当counter值大于等于pwm时，将IO口置低
		{GPIO_SetBits(GPIOA,GPIO_Pin_1);}//	将PC14 PC15置为低电平
    else 
		{GPIO_ResetBits(GPIOA,GPIO_Pin_1);}//	将PC14 PC15置为低电平*/
		
		
		
	
	}
void TIM_DIS_Init(){
	
	//跟测距有关初始化
	//用tim4
	/*********
	PA1  作控制端  接Trig
	PA2  作接收端  接Echo
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
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource2);  // 选定GPIOA的Pin2作为外部中断输入源
	
	TIM_TimeBaseStructure.TIM_Period = 9; 	 //10us
	TIM_TimeBaseStructure.TIM_Prescaler = 71; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	
	 TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);		 //	 初始化tim4
    TIM_ClearITPendingBit(TIM4,TIM_IT_Update); //清除TIM4溢出中断标志
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);   //  使能TIM4的溢出更新中断
    TIM_Cmd(TIM4,ENABLE);				 //		   使能TIM4
	
//	
  EXTI_InitStructure.EXTI_Line = EXTI_Line1;		//配置中断线路为12和13
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;        //配置为中断模式
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;     //配置为上升沿触发中断
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                  //配置中断为使能
  EXTI_Init(&EXTI_InitStructure);                            //初始化
}



void nvic()								   //配置中断优先级
{	 
     NVIC_InitTypeDef NVIC_InitStructure;		 //	  命名一优先级变量


		 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 	 //	   将优先级分组方式配置为group1,有2个抢占（打断）优先级，8个响应优先级
     NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;  //	该中断为10-15线路的外部中断
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //打断优先级为1，在改组中为较低的，0优先级最高
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			// 响应优先级0，打断优先级一样时，他最高
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//	设置使能
     NVIC_Init(&NVIC_InitStructure);							//	初始化
	  
 	   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);    //	   将优先级分组方式配置为group1,有2个抢占（打断）优先级，8个响应优先级
     NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; //该中断为TIM4溢出更新中断
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//打断优先级为1，在该组中为较低的，0优先级最高
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // 响应优先级0，打断优先级一样时，0最高
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	 	//	设置使能
     NVIC_Init(&NVIC_InitStructure);					   	//	初始化

	 
 	   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //要用同一个Group
     NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //TIM3	溢出更新中断
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//	打断优先级为1，与上一个相同，不希望中断相互打断对方
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 	//	响应优先级1，低于上一个，当两个中断同时来时，上一个先执行
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);

 
}
