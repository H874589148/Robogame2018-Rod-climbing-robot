#ifndef __RFID_H
#define __RFID_H	 
#include "sys.h"


u8 RFID_READ(u8 *uid,u8 *block);//��ȡrfid��һ������
u8 RFID_Init(void);//��ʼ����������ʹ������
u8 RFID_Disableantenna(void);//�ر����ߣ����ٷ���
u8 GET_UID(u8 *uid);//��ȡUID

		 				    
#endif