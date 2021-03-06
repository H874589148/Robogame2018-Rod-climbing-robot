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
void RCC_Configuration(void);	 //时钟初始化，开启外设时钟
void GPIO_Configuration(void);	 //IO口初始化，配置其功能
void delay_nus(u32);           //72M时钟下，约延时us
void delay_nms(u32);            //72M时钟下，约延时ms
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	 //使能PG端口时钟

	/*GPIOA*/
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	 //配置被选中的管脚，|表示同时被选中
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	   //配置IO口工作模式为	浮空输入
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   //配置IO口最高的输出速率为50M
  //GPIO_Init(GPIOA, &GPIO_InitStructure);		  //初始化GPIOC的相应IO口为上述配置，用于检测前后四个红外
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_11; //TIM3 CH2  TIM1 CH1~CH4舵机及单片机TX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	
	/*GPIOB*/
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6| GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //TIM3 CH2  TIM1 CH1~CH4
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*GPIOG*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //继电器-->PG.13 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOG, &GPIO_InitStructure);					 //根据设定参数初始化GPIOG.13
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 //配置IO口的工作模式为上拉输入（该io口内部外接电阻到电源）
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //配置IO口最高的输出速率为50M
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_11;  //配置被选中的管脚，|表示同时被选中
  GPIO_Init(GPIOG, &GPIO_InitStructure);			        //初始化GPIOC的相应IO口为上述配置，用于按键检测，�;
}
int main(void)
{
	//u8 len;	
	//u16 times=0;
	//u8 data[100];
	//u8 uid[8]={0};
	//u8 block[4]={0};	  
	//NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	//uart_init(19200);	 //串口1初始化为19200
	//uart_send_mydata(uid,8);
	//DIS_INT();
	RCC_GPIO_Configuration();
	TIM_PWM_Init();
	setAngle(1,90);
	while(1)
	{ 
		/*t=RFID_Init();
		delay_nms(1000);
		tt=GET_UID(uid);//获取UID
		delay_nms(1000);
		ttt=RFID_READ(uid,block);//读取rfid第一块数据
		delay_nms(1000);
		tttt=RFID_Disableantenna();//关闭天线，减少发热 
		delay_nms(1000);*/
		;
	
 }
  
}
void pagan_init(void){
		GPIO_SetBits(GPIOG,GPIO_Pin_13);//抱紧杆
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