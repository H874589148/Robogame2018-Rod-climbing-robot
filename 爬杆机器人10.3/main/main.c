//STLINK PA13 PA14
//RFID 初始化成功，读UID和BLOCK失败(位数问题）  需要PA9 PA10
//舵机两个手臂成功控制  PA7 PA8  旋转的舵机有一点问题PA11控制（360度,好像不大一样）
//测距成功，数据暂存在distance.c的m中，0.17*m即为距离，需考虑如何将m同步到main.c，PA1,PA2 一个模块需要两个IO口 需要主函数输出控制波，定时器不够了
//电机旋转 PB6-9 67控制1正反，89控制2正反转，V可取0-99 代表速度
//PA0读黑白，写在了Distance里 低为黑
#include "stm32f10x.h"
#include "math.h"
#include "stdio.h"
#include "delay.h"
#include "SteeringEngine.h"
#include "Distance.h"
#include "sys.h"
#include "usart.h"
#include "rfid.h"
//ALIENTEK战舰STM32开发板实验4
//串口实验  
//技术支持：www.openedv.com
//广州市星翼电子科技有限公司 
u8 RxData=0x00;
u8 flag=0;
u8 flag_count=0;
u8 f1=1,f2=1;
void RCC_Configuration(void){	 //时钟初始化，开启外设时
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG , ENABLE);//
}
void GPIO_Configuration(void){	 //IO口初始化，配置其功能
  GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_11; //TIM3 CH2  TIM1 CH1~CH4
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6| GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10; //TIM3 CH2  TIM1 CH1~CH4
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
}
void USART3_RCC_Configuration(void){
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
 //  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   //打开GPIOA,AFIO,USART1外设时钟
  
}
void nvic(void){
     NVIC_InitTypeDef NVIC_InitStructure;  //	 //	  命名一优先级变量

 	   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);    //	   将优先级分组方式配置为group1,有2个抢占（打断）优先级，8个响应优先级
     NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; //该中断为TIM2溢出更新中断
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//打断优先级为1，在该组中为较低的，0优先级最高
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // 响应优先级0，打断优先级一样时，0最高
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	 	//	设置使能
     NVIC_Init(&NVIC_InitStructure);					   	//	初始化

	 	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);    //	   将优先级分组方式配置为group1,有2个抢占（打断）优先级，8个响应优先级
     NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //该中断为TIM2溢出更新中断
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//打断优先级为1，在该组中为较低的，0优先级最高
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; // 响应优先级0，打断优先级一样时，0最高
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	 	//	设置使能
     NVIC_Init(&NVIC_InitStructure);					   	//	初始化 
	
 	   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //要用同一个Group
     NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn; //TIM1	溢出更新中断
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//	打断优先级为1，与上一个相同，不希望中断相互打断对方
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 	//	响应优先级1，低于上一个，当两个中断同时来时，上一个先执行
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);
	
	   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //要用同一个Group
     NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; //TIM3	溢出更新中断
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//	打断优先级为1，与上一个相同，不希望中断相互打断对方
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 	//	响应优先级1，低于上一个，当两个中断同时来时，上一个先执行
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);
}
void USART3_Configuration(void){
	  USART_InitTypeDef USART_InitStructure; //串口设置恢复默认参数
  
	  USART3_RCC_Configuration();
	  nvic();
	  
	  USART_InitStructure.USART_BaudRate = 38400; //波特率9600
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长8位
    USART_InitStructure.USART_StopBits = USART_StopBits_1; //1位停止字节
    USART_InitStructure.USART_Parity = USART_Parity_No; //无奇偶校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;     //无流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;      //打开Rx接收和Tx发送功能
    USART_Init(USART3, &USART_InitStructure);  //初始化
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  // 若接收数据寄存器满，则产生中断
    USART_Cmd(USART3, ENABLE);                     //启动串口                   
                                                   
       //-----如下语句解决第1个字节无法正确发送出去的问题-----// 
    USART_ClearFlag(USART3, USART_FLAG_TC);     // 清发送完成标志
}
void delay_nus(u32);           //72M时钟下，约延时us
void delay_nms(u32);            //72M时钟下，约延时ms
void pagan_init(void);
void duoji_1(void);
void duoji_2(void);
void duoji_3(void);
void duoji_4(void);
void duoji_5(void);
void duoji_open(u16 num);
void duoji_close(u16 num);
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
	USART3_Configuration();
	duoji_1();
	duoji_open(1);
	delay_nus(2000);
	duoji_open(3);
	delay_nus(2000);
  while(1)
	{ 
		if(flag_count==2)
			flag_count=0;
		if(flag==1){
			f2=GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_12);
			if(f2==0){
				delay_nus(20);
				f2=GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_12);
				if(f2==0){
				duoji_3();
				delay_nms(2600);
				duoji_open(1);
				duoji_open(3);
				STOP();
				}
				//下杆操作或停机操作
			}
		}
	
 }
  
}
void pagan_init(void){
	setWheel(2,2,100);
	delay_nms(1000);
	setWheel(1,1,100);//抱紧杆
	  setWheel(2,1,90);
	  setWheel(2,2,100);
    flag=1;
}	
void duoji_open(u16 num){
	setAngle(num,0);
}
void duoji_close(u16 num){
	setAngle(num,170);
	delay_nms(2000);
}
void duoji_1(void){
  setAngle(2 ,0);
	delay_nms(2000);
}
void duoji_2(void){
	setAngle(2 ,140);
	delay_nms(2000);
}
void duoji_3(void){
	setAngle(2,72);
	delay_nms(2000);
}
void duoji_4(void){
	setAngle(2,36);
	delay_nms(2000);
}
void duoji_5(void){
	setAngle(2,100);
  delay_nms(2000);
}
void STOP(void){
	setWheel(2,1,0);
	setWheel(2,2,0);
}	
void choose1(void){
    if(flag_count==1){
			duoji_1();
		}
}
void choose2(void){
	  if(flag_count==0){
			duoji_2();
			delay_nms(5000);
		}
}
void USART3_IRQHandler(void){
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET) //判断是否为接收中断
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);	//清除接受中断标志位
		RxData=USART_ReceiveData(USART3);								//读取接受数据{
			switch(RxData)
			{
				case (u8)0x61: USART_SendData(USART3, 'S');break;
				case (u8)0x41: duoji_close(1);delay_nms(3000);flag_count++;USART_SendData(USART3, 'T');duoji_2();break;
				case (u8)0x42: choose2();duoji_close(3);delay_nms(3000);flag_count++;USART_SendData(USART3, 'T');choose1();break;
				case (u8)0x50: duoji_4();delay_nms(5000);duoji_open(1);delay_nms(5000);duoji_5();delay_nms(5000);duoji_open(3);delay_nms(5000);USART_SendData(USART3, 'T');duoji_1();break;
				case (u8)0x44: pagan_init();break;
				default:break;
			}
		
	}
	
	
	if(USART_GetITStatus(USART3,USART_IT_TXE)!=RESET)  //判断是否为发送中断
	{	
		USART_ClearITPendingBit(USART3,USART_IT_TXE);//清除发送中断标志位
		
	}
	if(USART_GetITStatus(USART3,USART_IT_TC)!=RESET)  //判断是否为发送完成中断
	{
		USART_ClearITPendingBit(USART3,USART_IT_TC);	//清除发送完成中断标志位
	}

 }	 