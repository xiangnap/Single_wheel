#define __BLUETOOTH_C_
#include "includes.h"


int g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
int g_remote_frame_cnt = 0;
int g_start_PCcontrol=0;
BYTE remote_frame_data[REMOTE_FRAME_LENGTH];
BYTE remote_frame_data_send[REMOTE_FRAME_LENGTH];
//BYTE g_device_NO = WIFI_ADDRESS_CAR_2;	/* 设备号 即WiFi地址 */
SWORD ans=0;


/*-----------------------------------------------------------------------*/
/* 执行远程命令                                                          */
/*-----------------------------------------------------------------------*/
void execute_remote_cmd(const BYTE *data)
{
	BYTE cmd = 0;
	
	cmd = (BYTE)(data[0]);
	switch (cmd)
	{
		
		/* 开始上位机控制 */
		case CMD_START_CONTROL:
			g_start_PCcontrol=1;
		break;
		
		/* 上位机控制结束 */
		case CMD_STOP_CONTROL:
			g_start_PCcontrol=0;
		break;
		
		/* 俯仰陀螺仪标定调参 */
		case CMD_SET_PITCH_ANGLE_ZERO :
			set_PITCH_angle_zero(*((SWORD *)(&(data[2]))));
			D7=~D7;
		break;
		case CMD_SET_PITCH_ANGLE_SPEED_ZERO :
			set_PITCH_angle_speed_zero(*((SWORD *)(&(data[2]))));
		break;
		
		case CMD_SET_ANGLE_KP :
			set_angle_KP(*((SWORD *)(&(data[2]))));
		break;
		case CMD_SET_ANGLE_KI :
			set_angle_KI(*((SWORD *)(&(data[2]))));
		break;
		case CMD_SET_ANGLE_KD :
			set_angle_KD(*((SWORD *)(&(data[2]))));
		break;
		
		/* 横滚陀螺仪标定调参 */
		case CMD_SET_ROLL_ANGLE_ZERO :
			set_ROLL_angle_zero(*((SWORD *)(&(data[2]))));
		break;
		case CMD_SET_ROLL_ANGLE_SPEED_ZERO :
			set_ROLL_angle_speed_zero(*((SWORD *)(&(data[2]))));
		break;
		
		/* 航向角陀螺仪标定调参 */
		case CMD_SET_YAW_ANGLE_ZERO :
			set_YAW_angle_zero(*((SWORD *)(&(data[2]))));
		break;
		case CMD_SET_YAW_ANGLE_SPEED_ZERO :
			set_YAW_angle_speed_zero(*((SWORD *)(&(data[2]))));
			D7=~D7;
		break;
		
		
		/* 驱动轮电机调参 */
		case CMD_SET_MOTOR1_TARGET :
			set_speed_target(*((SWORD *)(&(data[2]))));
		break;
		case CMD_STOP_SPEED :
			set_speed_target((SWORD)0);
		break;
		case CMD_SET_MOTOR1_PWM_TARGET:
			set_pwm1_target(*((SWORD *)(&(data[2]))));
		break;
		
		case CMD_SET_MOTOR1_KP :
			set_speed_KP(*((SWORD *)(&(data[2]))));
		break;
		case CMD_SET_MOTOR1_KI :
			set_speed_KI(*((SWORD *)(&(data[2]))));
		break;
		case CMD_SET_MOTOR1_KD :
			set_speed_KD(*((SWORD *)(&(data[2]))));
		break;

		
		
		/* 左右平衡电机调参 	*/
		case CMD_SET_MOTOR2_PWM_TARGET :
			set_pwm2_target(*((SWORD *)(&(data[2]))));
		break;
		
		/* 航向角电机调参 	*/
		case CMD_SET_MOTOR3_PWM_TARGET :
			set_pwm3_target(*((SWORD *)(&(data[2]))));
		break;
		
#if 0		
		/* 保留   用于通信	*/
		case CMD_NET :
		g_net_control_data.is_new_cmd = 1;
		g_net_control_data.cmd = *((WORD *)(&(data[2])));
		break;
#endif		

	}
}


/*-----------------------------------------------------------------------*/
/* 接受远程数据帧                                                        */
/* 第二版                                                                                              */
/* 5A 命令类型(1B) 校验字(1B)              */
/* 未改变原数据帧的大部分定义及长度                                      */
/*-----------------------------------------------------------------------*/
int rev_remote_frame_2(BYTE rev)
{
	BYTE Data[3];
	if (g_remote_frame_cnt == 0)
	{
		Data[0]=rev;
		if(rev==0xA5)
			D0=~D0;
		g_remote_frame_cnt++;
	}
	else if(g_remote_frame_cnt==1)
	{
		Data[1]=rev;
		if(rev==0x57)
			D1=~D1;
		g_remote_frame_cnt++;
	}
	else if(g_remote_frame_cnt==2)
	{
		Data[2]=rev;
		if(rev==0xFC)
			D2=~D2;
		g_remote_frame_cnt=0;
		LCD_Write_Num(80,1,Data[0],5);
		LCD_Write_Num(80,3,Data[1],5);
		LCD_Write_Num(80,5,Data[2],5);
	}
//	BYTE Data[3];
//	uint8_t sum;
//	if (g_remote_frame_cnt == 0)	//接受起始位
//	{
//		if (rev == 0x5A)
//		{
//			remote_frame_data[g_remote_frame_cnt++] = 0x5A;
//		}
//	}
//	else if (g_remote_frame_cnt == 1)	//接受命令类型位
//	{
//		if (rev == 0x57)	//加陀校准
//		{
//			D1=~D1;
//			remote_frame_data[g_remote_frame_cnt++] = 0x57;
//		}
//		else if(rev==0x58)	//磁力校准
//		{
//			D2=~D2;
//			remote_frame_data[g_remote_frame_cnt++] = 0x58;
//		}
//		else if(rev==0x85)//读取量程
//		{
//			D3=~D3;
//			remote_frame_data[g_remote_frame_cnt++] = 0x85;
//		}
//		else if(rev==0x75)//精度，频率
//		{
//			remote_frame_data[g_remote_frame_cnt++] = 0x75;
//		}
//		else
//		{
//			g_remote_frame_cnt=0;
//		}
//	}
//	else if (g_remote_frame_cnt == 2)	//接受校验位
//	{
//		sum=(uint8_t)(remote_frame_data[0]+remote_frame_data[1]);
//		if(rev==sum)
//		{
//			remote_frame_data[g_remote_frame_cnt] = rev;
//			g_remote_frame_state = REMOTE_FRAME_STATE_OK;	//CheckSum Success
//			if(remote_frame_data[1]==0x57)
//			{
//				GY953_Write(0x02,0x13);
//			}
//			else if(remote_frame_data[1]==0x58)	
//			{
//				send_data2PC(3, PREC_TYPE, Data);
//				GY953_Write(0x02,0x13);
//			}
//			else if(remote_frame_data[1]==0x75)
//			{
//				Read_Precision(Data);
//				send_data2PC(3, PREC_TYPE, Data);
//			}
//		}
//		g_remote_frame_cnt=0;
//	}
//
//	return g_remote_frame_state;
}


/*-----------------------------------------------------------------------*/
/* 生成并发送数据至上位机                                                    */
/* 5A 5A 数据类型(1B) 长度(1B) 数据体   校验和(1B)	*/
/* 参数 :   type 发送数据类型                                                 */
/*        length data长度                                                */
/*        data发出的数据体                                   */
/*		     校验位除自身外全部位的和校验   */
/*-----------------------------------------------------------------------*/
void generate_remote_frame_2(BYTE type, BYTE length, const BYTE data[])
{
	WORD i = 0, j = 0;
	D7=~D7;
	remote_frame_data_send[i++] = 0x5A;
	remote_frame_data_send[i++] = 0x5A;
	remote_frame_data_send[i++] = type;
	remote_frame_data_send[i++] = length;
	for (j = 0; j < length; j++)
	{
		remote_frame_data_send[i++] = data[j];
	}
	remote_frame_data_send[i] = check_sum(remote_frame_data_send, length+4);
	
	serial_port_1_TX_array(remote_frame_data_send, length+5);
}


/*-----------------------------------------------------------------------*/
/* 发送GY953数据至PC上位机                                                          */
/* 依赖generate_remote_frame_2()                                          */
/* sensor传感器类型
/* datatype数据类型
/*-----------------------------------------------------------------------*/
void send_data2PC(BYTE sensor, BYTE type, BYTE data[])
{
	if(sensor==ENC03)
	{D6=~D6;
		if(type==GYR_TYPE)
			generate_remote_frame_2( type, 2, (const BYTE *)(&data[1]));
		else if(type==ANGLE_TYPE)
			generate_remote_frame_2( type, 2, (const BYTE *)(&data[0]));
	}
	else if(sensor==MPU9250)
	{
		if(type==ACC_TYPE)
			generate_remote_frame_2( type, 6, (const BYTE *)(&data[6]));
		else if(type==GYR_TYPE)
			generate_remote_frame_2( type, 6, (const BYTE *)(&data[0]));
	}
	else if(sensor==GY953)
		{
			if(type==ACC_TYPE)
				generate_remote_frame_2( type, 6, (const BYTE *)(&data[2]));
			else if(type==GYR_TYPE)
				generate_remote_frame_2( type, 6, (const BYTE *)(&data[8]));
			else if(type==ANGLE_TYPE)
				generate_remote_frame_2( type, 6, (const BYTE *)(&data[20]));
			else if(type==MAG_TYPE)
				generate_remote_frame_2( type, 6, (const BYTE *)(&data[14]));
			else if(type==FOUR_TYPE)
				generate_remote_frame_2( type, 8, (const BYTE *)(&data[26]));
		}
	
}


/*-----------------------------------------------------------------------*/
/* 异或校验                                                                            */
/*-----------------------------------------------------------------------*/
BYTE check_xor(const BYTE *data, BYTE length)
{
	int i;
	BYTE res = 0;
	
	for (i=0; i<length; i++)
	{
		res ^= data[i];
	}
	
	return res;
}

/*-----------------------------------------------------------------------*/
/* 累加和校验                                                                            */
/*-----------------------------------------------------------------------*/
BYTE check_sum(const BYTE *data, BYTE length)
{
	int i;
	WORD res = 0;
	
	for (i=0; i<length; i++)
	{
		res += data[i];
		res  = res&0x00FF;
	}
	
	return (BYTE)res;
}



