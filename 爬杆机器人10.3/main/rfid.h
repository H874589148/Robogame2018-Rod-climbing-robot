#ifndef __RFID_H
#define __RFID_H	 
#include "sys.h"


u8 RFID_READ(u8 *uid,u8 *block);//读取rfid第一块数据
u8 RFID_Init(void);//初始化读卡器，使能天线
u8 RFID_Disableantenna(void);//关闭天线，减少发热
u8 GET_UID(u8 *uid);//获取UID

		 				    
#endif