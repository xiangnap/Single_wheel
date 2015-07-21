#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

/* ����PITCH������Զ�̱궨 */
#define CMD_SET_PITCH_ANGLE_ZERO (0x02)	/* Զ�̱궨���������ǽǶ���� */
#define CMD_SET_PITCH_ANGLE_SPEED_ZERO (0x03)	/* Զ�̱궨���������ǽ��ٶ���� */

/* ���ROLL������Զ�̱궨 */
#define CMD_SET_ROLL_ANGLE_ZERO (0x04)	/* Զ�̱궨��������ǽǶ���� */
#define CMD_SET_ROLL_ANGLE_SPEED_ZERO (0x05)	/* Զ�̱궨��������ǽ��ٶ���� */

/* ����YAW������Զ�̱궨 */
#define CMD_SET_YAW_ANGLE_ZERO (0x06)	/* Զ�̱궨���������ǽǶ���� */
#define CMD_SET_YAW_ANGLE_SPEED_ZERO (0x07)	/* Զ�̱궨���������ǽ��ٶ���� */

/* �����ֵ��Զ�̵��� */
#define CMD_SET_MOTOR1_TARGET (0x08)	/* Զ���޸ĵ��Ŀ��ֵ */
#define CMD_SET_MOTOR1_PWM_TARGET (0x09)	/* Զ���޸ĵ��ռ�ձ� */
#define CMD_SET_MOTOR1_KP (0x10)	/* Զ���޸ĵ��Pֵ */
#define CMD_SET_MOTOR1_KI (0x11)	/* Զ���޸ĵ��Iֵ */
#define CMD_SET_MOTOR1_KD (0x12)	/* Զ���޸ĵ��Dֵ */
#define CMD_STOP_SPEED (0x13)	  /* Զ�̵��Ŀ��ֵ��0 */

/* ������Զ�̵��� */
#define CMD_SET_MOTOR2_PWM_TARGET (0x14)	/* Զ���޸ĵ��ռ�ձ� */

/* ����ǵ��Զ�̵��� */
#define CMD_SET_MOTOR3_PWM_TARGET (0x15)	/* Զ���޸ĵ��ռ�ձ� */

/* ��λ������ */
#define CMD_START_CONTROL (0x20)/* ��ʼPC���� */
#define CMD_STOP_CONTROL (0x21)	/* ����PC���� */

/* ��װ֡���� */
#define REMOTE_FRAME_LENGTH (24)//����޸�
#define REMOTE_FRAME_STATE_OK (1)
#define REMOTE_FRAME_STATE_NOK (0)

/* ������������ */
#define ACC_TYPE (0x15)
#define GYR_TYPE (0x25)
#define MAG_TYPE (0x35)
#define ANGLE_TYPE (0x45)
#define FOUR_TYPE (0x65)
#define PREC_TYPE (0x75)
#define RANGE_TYPE (0x85)

/* ���������� */
#define ENC03 (1)
#define MPU9250 (2)
#define GY953 (3)


extern BYTE remote_frame_data[];
extern int g_remote_frame_state;
extern BYTE g_device_NO;
extern int g_start_PCcontrol;
extern SWORD ans;



/* ������ƽṹ�� */
#ifdef __BLUETOOTH_C_
struct
{
	int is_new_cmd;
	WORD cmd;
	DWORD last_report_online_time;
} g_net_control_data = { 0, 0x00, 0x00000000 };
#else
extern struct
{
	int is_new_cmd;
	WORD cmd;
	DWORD last_report_online_time;
} g_net_control_data;
#endif

BYTE check_xor(const BYTE *data, BYTE length);
BYTE check_sum(const BYTE *data, BYTE length);
extern void execute_remote_cmd(const BYTE *data);
extern int rev_remote_frame_2(BYTE rev);
extern void generate_remote_frame_2(BYTE type, BYTE length, const BYTE data[]);
extern void send_data2PC(BYTE sensor, BYTE type, BYTE data[]);
void report_online(void);




#endif /* BLUETOOTH_H_ */