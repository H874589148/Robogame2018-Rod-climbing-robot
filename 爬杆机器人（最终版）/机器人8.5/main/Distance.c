#include "stm32f10x.h"
#include "delay.h"
	
  u32 m,black=0;
 void FUN(float dis){
	 dis=0.17*m;}
	 
void EXTI1_IRQHandler(void)							   //  线聲1外部中断的中断响应函数
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
	
	GPIO_InitTypeDef GPIO_InitStructure;   //定义格式为GPIO_InitTypeDef的结构体的名字为GPIO_InitStructure
	EXTI_InitTypeDef EXTI_InitStructure;				   //命名一中断变量
	 NVIC_InitTypeDef NVIC_InitStructure;		 //	  命名一优先级变量
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	  //定义格式为TIM_TimeBaseInitTypeDef的结构体的名字为TIM_TimeBaseStructure  
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	  //使能APB2控制外设的时钟，包括GPIOC, 功能复用时钟AFIO等，
                                                                              //其他包括的外设，详见固件库手册等资料
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
                



  
	    
                                       	  //typedef struct { u16 GPIO_Pin; GPIOSpeed_TypeDef GPIO_Speed; GPIOMode_TypeDef GPIO_Mode; } GPIO_InitTypeDef;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	 //配置IO口的工作模式推挽输出(io口内部外接电阻到电源）
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //配置IO口最高的输出速率为50M
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;  //配置被选中的管脚，|表示同时被选中
    GPIO_Init(GPIOA, &GPIO_InitStructure);			        //初始化GPIOB的相应IO口为上述配置，用于按键检测，本demo通过外部中断暯式实现
   

	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 //配置IO口的工作模式为上拉输入（该io口内部外接电阻到电源）
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //配置IO口最高的输出速率为50M
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;  //配置被选中的管脚，|表示同时被选中
    GPIO_Init(GPIOA, &GPIO_InitStructure);			        //初始化GPIOC的相应IO口为上述配置，用于按键检测，本demo通过外部中断暯式实现
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);  // 选定GPIOB的Pin1作为外部中断输入源


GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 //配置IO口的工作模式为上拉输入（该io口内部外接电阻到电源）
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //配置IO口最高的输出速率为50M
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;  //配置被选中的管脚，|表示同时被选中
    GPIO_Init(GPIOA, &GPIO_InitStructure);			        //初始化GPIOC的相应IO口为上述配置，用于按键检测，本demo通过外部中断暯式实现


  

	EXTI_InitStructure.EXTI_Line = EXTI_Line1;		//配置中断线聲为1
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;        //配置为中断模式
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;     //配置为上升和下降沿触暍
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;                  //配置中断为使能
  EXTI_Init(&EXTI_InitStructure);                            //初始化
	


 
    

		 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 	 //	   将优先级曋组暯式配置为group1,有2个抢占（打断）优先级，8个响应优先级
     NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;  //	该中断为1线聲的外部中断
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; //打断优先级为1，在改组中为较低的，0优先级最高
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			// 响应优先级0，打断优先级一样时，他最高
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//	设置使能
     NVIC_Init(&NVIC_InitStructure);							//	初始化

     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);    //	   将优先级曋组暯式配置为group1,有2个抢占（打断）优先级，8个响应优先级
     NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; //该中断为TIM4溢出更新中断
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//打断优先级为1，在该组中为较低的，0优先级最高
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1 ;// 响应优先级0，打断优先级一样时，0最高
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	 	//	设置使能
     NVIC_Init(&NVIC_InitStructure);					   	//	初始化
 	   


  	

  	TIM_TimeBaseStructure.TIM_Period =65535;		  // 配置计数阈值
		TIM_TimeBaseStructure.TIM_Prescaler =71;		 //	 1us
  	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	// 时钟曋频倍数
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	// 计数暯式为向上计数
	  
	  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);		 //	 初始化tim4
    TIM_ClearITPendingBit(TIM2,TIM_IT_Update); //清除TIM4溢出中断标志
    TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);   //  失能TIM4的溢出更新中断
    TIM_Cmd(TIM2,ENABLE);				 //		   使能TIM4

	}

	