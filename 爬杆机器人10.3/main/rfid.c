#include "delay.h"
#include "usart.h"
#include "rfid.h"



u8 RFID_init_data[10] = {0xAA,0xBB,0x06,0x00,0x00,0x00,0x08,0x01,0x31,0x38};
u8 Get_uid_data[9] = {0xAA,0xBB,0x05,0x00,0x00,0x00,0x01,0x10,0x11};
u8 RFID_READ_data[20]={0xAA,0xBB,0x10,0x00,0x00,0x00,0x05,0x10,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00};
u8 Disable_antenna_data[10]={0xAA,0xBB,0x06,0x00,0x00,0x00,0x0C,0x01,0x00,0x0D};
u8 look;
	
	
	
u8 RFID_Init(void)
{
	u8 len=0;
	u8 i=0;
	u8 data[10]={0};
	uart_send_mydata(RFID_init_data,10);
	
	while(i<50)
		
		{
			if(USART_RX_STA&0x8000)
			{
				uart_receive_mydata(data,&len);
				break;
			}
			i++;
			delay_nms(10);
		}
			
		
	USART_RX_STA=0;
		
	if(len==10&&data[8]==0)
	{
		return 1;
	}
	else 
	{
		return 0;		
	}
	
}


u8 GET_UID(u8 *uid)
{
	
	u8 len;
	u8 i;
	u8 data[19]={0};
	uart_send_mydata(Get_uid_data,9);
	
	i=0;
	while(i<50)
		
		{
			if(USART_RX_STA&0x8000)
			{
				uart_receive_mydata(data,&len);
				break;
			}
			i++;
			delay_nms(10);
		}
			
		
	USART_RX_STA=0;
		look=len;
	if(len==19)
	{
		for(i=0;i<8;i++)
		uid[i]=data[i+10];
	 
		return 1;
	}
	else
	{
		return 0;
	}
			
	
}

u8 RFID_READ(u8 *uid,u8 *block)
{
	u8 i=0;
	u8 len=0;
	u8 data[14]={0};
	for(i=0;i<8;i++)
		RFID_READ_data[i+9]=uid[i];
	RFID_READ_data[19]=0;
	for(i=4;i<19;i++)
		RFID_READ_data[19]=RFID_READ_data[i]^RFID_READ_data[19];
	uart_send_mydata(RFID_READ_data,20);
	
	i=0;
	while(i<50)
		
		{
			if(USART_RX_STA&0x8000)
			{
				uart_receive_mydata(data,&len);
				break;
			}
			i++;
			delay_nms(10);
		}
			
		
	USART_RX_STA=0;
		
	if(len==14)
	{
		for(i=0;i<4;i++)
			block[i]=data[i+9];
		return 1;
	}
	else 
		return 0;
	
			
	
	
}

u8  RFID_Disableantenna(void)
{
	u8 len=0;
	u8 i=0;
	u8 data[10]={0};
	uart_send_mydata(Disable_antenna_data,10);
	
	while(i<50)
	
	{
		if(USART_RX_STA&0x8000)
		{
			uart_receive_mydata(data,&len);
			break;
		}
		i++;
		delay_nms(10);
	}
	
		USART_RX_STA=0;
	
	if(len==10&&data[8]==0)
	{
		
		return 1;
	}
	else 
	{
		return 0;		
	}
}
	