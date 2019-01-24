//STLINK PA13 PA14
//RFID ³õÊ¼»¯³É¹¦£¬¶ÁUIDºÍBLOCKÊ§°Ü(Î»ÊıÎÊÌâ£©  ĞèÒªPA9 PA10
//¶æ»úÁ½¸öÊÖ±Û³É¹¦¿ØÖÆ  PA7 PA8  Ğı×ªµÄ¶æ»úÓĞÒ»µãÎÊÌâPA11¿ØÖÆ£¨360¶È,ºÃÏñ²»´óÒ»Ñù£©
//²â¾à³É¹¦£¬Êı¾İÔİ´æÔÚdistance.cµÄmÖĞ£¬0.17*m¼´Îª¾àÀë£¬Ğè¿¼ÂÇÈçºÎ½«mÍ¬²½µ½main.c£¬PA1,PA2 Ò»¸öÄ£¿éĞèÒªÁ½¸öIO¿Ú ĞèÒªÖ÷º¯ÊıÊä³ö¿ØÖÆ²¨£¬¶¨Ê±Æ÷²»¹»ÁË
//µç»úĞı×ª PB6-9 67¿ØÖÆ1Õı·´£¬89¿ØÖÆ2Õı·´×ª£¬V¿ÉÈ¡0-99 ´ú±íËÙ¶È
//PA0¶ÁºÚ°×£¬Ğ´ÔÚÁËDistanceÀï µÍÎªºÚ
#include "stm32f10x.h"
#include "math.h"
#include "stdio.h"
#include "delay.h"
#include "SteeringEngine.h"
#include "Distance.h"
#include "sys.h"
#include "usart.h"
#include "rfid.h"
//ALIENTEKÕ½½¢STM32¿ª·¢°åÊµÑé4
//´®¿ÚÊµÑé  
//¼¼ÊõÖ§³Ö£ºwww.openedv.com
//¹ãÖİÊĞĞÇÒíµç×Ó¿Æ¼¼ÓĞÏŞ¹«Ë¾ 
void RCC_Configuration(void);	 //Ê±ÖÓ³õÊ¼»¯£¬¿ªÆôÍâÉèÊ±ÖÓ
void GPIO_Configuration(void);	 //IO¿Ú³õÊ¼»¯£¬ÅäÖÃÆä¹¦ÄÜ
void delay_nus(u32);           //72MÊ±ÖÓÏÂ£¬Ô¼ÑÓÊ±us
void delay_nms(u32);            //72MÊ±ÖÓÏÂ£¬Ô¼ÑÓÊ±ms
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	 //Ê¹ÄÜPG¶Ë¿ÚÊ±ÖÓ

	/*GPIOA*/
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	 //ÅäÖÃ±»Ñ¡ÖĞµÄ¹Ü½Å£¬|±íÊ¾Í¬Ê±±»Ñ¡ÖĞ
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	   //ÅäÖÃIO¿Ú¹¤×÷Ä£Ê½Îª	¸¡¿ÕÊäÈë
  //GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   //ÅäÖÃIO¿Ú×î¸ßµÄÊä³öËÙÂÊÎª50M
  //GPIO_Init(GPIOA, &GPIO_InitStructure);		  //³õÊ¼»¯GPIOCµÄÏàÓ¦IO¿ÚÎªÉÏÊöÅäÖÃ£¬ÓÃÓÚ¼ì²âÇ°ºóËÄ¸öºìÍâ
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_11; //TIM3 CH2  TIM1 CH1~CH4¶æ»ú¼°µ¥Æ¬»úTX
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	
	/*GPIOB*/
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6| GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //TIM3 CH2  TIM1 CH1~CH4
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*GPIOG*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //¼ÌµçÆ÷-->PG.13 ¶Ë¿ÚÅäÖÃ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO¿ÚËÙ¶ÈÎª50MHz
	GPIO_Init(GPIOG, &GPIO_InitStructure);					 //¸ù¾İÉè¶¨²ÎÊı³õÊ¼»¯GPIOG.13
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 //ÅäÖÃIO¿ÚµÄ¹¤×÷Ä£Ê½ÎªÉÏÀ­ÊäÈë£¨¸Ãio¿ÚÄÚ²¿Íâ½Óµç×èµ½µçÔ´£©
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //ÅäÖÃIO¿Ú×î¸ßµÄÊä³öËÙÂÊÎª50M
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_11;  //ÅäÖÃ±»Ñ¡ÖĞµÄ¹Ü½Å£¬|±íÊ¾Í¬Ê±±»Ñ¡ÖĞ
  GPIO_Init(GPIOG, &GPIO_InitStructure);			        //³õÊ¼»¯GPIOCµÄÏàÓ¦IO¿ÚÎªÉÏÊöÅäÖÃ£¬ÓÃÓÚ°´¼ü¼ì²â£¬±;
}
int main(void)
{
	//u8 len;	
	//u16 times=0;
	//u8 data[100];
	//u8 uid[8]={0};
	//u8 block[4]={0};	  
	//NVIC_Configuration(); 	 //ÉèÖÃNVICÖĞ¶Ï·Ö×é2:2Î»ÇÀÕ¼ÓÅÏÈ¼¶£¬2Î»ÏìÓ¦ÓÅÏÈ¼¶
	//uart_init(19200);	 //´®¿Ú1³õÊ¼»¯Îª19200
	//uart_send_mydata(uid,8);
	//DIS_INT();
	RCC_GPIO_Configuration();
	TIM_PWM_Init();
	setAngle(1,90);
	while(1)
	{ 
		/*t=RFID_Init();
		delay_nms(1000);
		tt=GET_UID(uid);//»ñÈ¡UID
		delay_nms(1000);
		ttt=RFID_READ(uid,block);//¶ÁÈ¡rfidµÚÒ»¿éÊı¾İ
		delay_nms(1000);
		tttt=RFID_Disableantenna();//¹Ø±ÕÌìÏß£¬¼õÉÙ·¢ÈÈ 
		delay_nms(1000);*/
		;
	
 }
  
}
void pagan_init(void){
		GPIO_SetBits(GPIOG,GPIO_Pin_13);//±§½ô¸Ë
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