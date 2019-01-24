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
u8 RxData=0x00;
u8 flag=0;
u8 flag_count=0;
u8 f1=1,f2=1;
void RCC_Configuration(void){	 //Ê±ÖÓ³õÊ¼»¯£¬¿ªÆôÍâÉèÊ±
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);//
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG , ENABLE);//
}
void GPIO_Configuration(void){	 //IO¿Ú³õÊ¼»¯£¬ÅäÖÃÆä¹¦ÄÜ
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
 //  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);   //´ò¿ªGPIOA,AFIO,USART1ÍâÉèÊ±ÖÓ
  
}
void nvic(void){
     NVIC_InitTypeDef NVIC_InitStructure;  //	 //	  ÃüÃûÒ»ÓÅÏÈ¼¶±äÁ¿

 	   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);    //	   ½«ÓÅÏÈ¼¶·Ö×é·½Ê½ÅäÖÃÎªgroup1,ÓĞ2¸öÇÀÕ¼£¨´ò¶Ï£©ÓÅÏÈ¼¶£¬8¸öÏìÓ¦ÓÅÏÈ¼¶
     NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; //¸ÃÖĞ¶ÏÎªTIM2Òç³ö¸üĞÂÖĞ¶Ï
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//´ò¶ÏÓÅÏÈ¼¶Îª1£¬ÔÚ¸Ã×éÖĞÎª½ÏµÍµÄ£¬0ÓÅÏÈ¼¶×î¸ß
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; // ÏìÓ¦ÓÅÏÈ¼¶0£¬´ò¶ÏÓÅÏÈ¼¶Ò»ÑùÊ±£¬0×î¸ß
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	 	//	ÉèÖÃÊ¹ÄÜ
     NVIC_Init(&NVIC_InitStructure);					   	//	³õÊ¼»¯

	 	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);    //	   ½«ÓÅÏÈ¼¶·Ö×é·½Ê½ÅäÖÃÎªgroup1,ÓĞ2¸öÇÀÕ¼£¨´ò¶Ï£©ÓÅÏÈ¼¶£¬8¸öÏìÓ¦ÓÅÏÈ¼¶
     NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //¸ÃÖĞ¶ÏÎªTIM2Òç³ö¸üĞÂÖĞ¶Ï
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//´ò¶ÏÓÅÏÈ¼¶Îª1£¬ÔÚ¸Ã×éÖĞÎª½ÏµÍµÄ£¬0ÓÅÏÈ¼¶×î¸ß
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; // ÏìÓ¦ÓÅÏÈ¼¶0£¬´ò¶ÏÓÅÏÈ¼¶Ò»ÑùÊ±£¬0×î¸ß
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	 	//	ÉèÖÃÊ¹ÄÜ
     NVIC_Init(&NVIC_InitStructure);					   	//	³õÊ¼»¯ 
	
 	   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //ÒªÓÃÍ¬Ò»¸öGroup
     NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn; //TIM1	Òç³ö¸üĞÂÖĞ¶Ï
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//	´ò¶ÏÓÅÏÈ¼¶Îª1£¬ÓëÉÏÒ»¸öÏàÍ¬£¬²»Ï£ÍûÖĞ¶ÏÏà»¥´ò¶Ï¶Ô·½
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; 	//	ÏìÓ¦ÓÅÏÈ¼¶1£¬µÍÓÚÉÏÒ»¸ö£¬µ±Á½¸öÖĞ¶ÏÍ¬Ê±À´Ê±£¬ÉÏÒ»¸öÏÈÖ´ĞĞ
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);
	
	   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //ÒªÓÃÍ¬Ò»¸öGroup
     NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; //TIM3	Òç³ö¸üĞÂÖĞ¶Ï
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//	´ò¶ÏÓÅÏÈ¼¶Îª1£¬ÓëÉÏÒ»¸öÏàÍ¬£¬²»Ï£ÍûÖĞ¶ÏÏà»¥´ò¶Ï¶Ô·½
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 	//	ÏìÓ¦ÓÅÏÈ¼¶1£¬µÍÓÚÉÏÒ»¸ö£¬µ±Á½¸öÖĞ¶ÏÍ¬Ê±À´Ê±£¬ÉÏÒ»¸öÏÈÖ´ĞĞ
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStructure);
}
void USART3_Configuration(void){
	  USART_InitTypeDef USART_InitStructure; //´®¿ÚÉèÖÃ»Ö¸´Ä¬ÈÏ²ÎÊı
  
	  USART3_RCC_Configuration();
	  nvic();
	  
	  USART_InitStructure.USART_BaudRate = 38400; //²¨ÌØÂÊ9600
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //×Ö³¤8Î»
    USART_InitStructure.USART_StopBits = USART_StopBits_1; //1Î»Í£Ö¹×Ö½Ú
    USART_InitStructure.USART_Parity = USART_Parity_No; //ÎŞÆæÅ¼Ğ£Ñé
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;     //ÎŞÁ÷¿ØÖÆ
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;      //´ò¿ªRx½ÓÊÕºÍTx·¢ËÍ¹¦ÄÜ
    USART_Init(USART3, &USART_InitStructure);  //³õÊ¼»¯
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);  // Èô½ÓÊÕÊı¾İ¼Ä´æÆ÷Âú£¬Ôò²úÉúÖĞ¶Ï
    USART_Cmd(USART3, ENABLE);                     //Æô¶¯´®¿Ú                   
                                                   
       //-----ÈçÏÂÓï¾ä½â¾öµÚ1¸ö×Ö½ÚÎŞ·¨ÕıÈ··¢ËÍ³öÈ¥µÄÎÊÌâ-----// 
    USART_ClearFlag(USART3, USART_FLAG_TC);     // Çå·¢ËÍÍê³É±êÖ¾
}
void delay_nus(u32);           //72MÊ±ÖÓÏÂ£¬Ô¼ÑÓÊ±us
void delay_nms(u32);            //72MÊ±ÖÓÏÂ£¬Ô¼ÑÓÊ±ms
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
				STOP();//ÏÂ¸Ë²Ù×÷»òÍ£»ú²Ù×
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
	if(USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET) //ÅĞ¶ÏÊÇ·ñÎª½ÓÊÕÖĞ¶Ï
	{
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);	//Çå³ı½ÓÊÜÖĞ¶Ï±êÖ¾Î»
		RxData=USART_ReceiveData(USART3);								//¶ÁÈ¡½ÓÊÜÊı¾İ{
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
	
	
	if(USART_GetITStatus(USART3,USART_IT_TXE)!=RESET)  //ÅĞ¶ÏÊÇ·ñÎª·¢ËÍÖĞ¶Ï
	{	
		USART_ClearITPendingBit(USART3,USART_IT_TXE);//Çå³ı·¢ËÍÖĞ¶Ï±êÖ¾Î»
		
	}
	if(USART_GetITStatus(USART3,USART_IT_TC)!=RESET)  //ÅĞ¶ÏÊÇ·ñÎª·¢ËÍÍê³ÉÖĞ¶Ï
	{
		USART_ClearITPendingBit(USART3,USART_IT_TC);	//Çå³ı·¢ËÍÍê³ÉÖĞ¶Ï±êÖ¾Î»
	}

 }	 