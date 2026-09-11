/*
 * Foundation.h
 *
 *  Created on: 2026年1月26日
 *      Author: zjx
 */

#ifndef CODE_FOUNDATION_H_
#define CODE_FOUNDATION_H_

#define Buzzer_pin  P33_10

#define KEY1        P20_6
#define KEY2        P20_7
#define KEY3        P11_2
#define KEY4        P11_3

#define Switch1     P33_11
#define Switch2     P33_12

#define LED1                    (P20_9)
#define LED2                    (P20_8)
#define LED3                    (P21_5)
#define LED4                    (P21_4)



//IPS200宏定义     240*320
#define ips_show_string                  ips200_show_string
#define ips_show_int                     ips200_show_int
#define ips_show_uint                    ips200_show_uint
#define ips_show_float                   ips200_show_float
#define ips_show_chinese                 ips200_show_chinese
#define ips_clear                        ips200_clear
#define ips_init                         ips200_init
#define ips_show_rgb565_image            ips200_show_rgb565_image


#define M_MAX    5000
#define M_MIN   -5000

//#define ENCODER_1                   (TIM6_ENCODER)
//#define ENCODER_1_A                 (TIM6_ENCODER_CH1_P20_3)
//#define ENCODER_1_B                 (TIM6_ENCODER_CH2_P20_0)

#define ENCODER_1                   (TIM2_ENCODER)
#define ENCODER_1_A                 (TIM2_ENCODER_CH1_P33_7)
#define ENCODER_1_B                 (TIM2_ENCODER_CH2_P33_6)

#define SERVO_MOTOR_PWM             (ATOM1_CH1_P33_9)                           // 定义主板上舵机对应引脚
#define SERVO_MOTOR_FREQ            (300)                                        // 定义主板上舵机频率


#define SERVO_MOTOR_MID             (146)
#define SERVO_MOTOR_LMAX            (96)
#define SERVO_MOTOR_RMAX            (196)

#define SERVO_MOTOR_DUTY(x)         ((float)PWM_DUTY_MAX/(1000.0/(float)SERVO_MOTOR_FREQ)*(0.5+(float)(x)/90.0))// ------------------ 舵机占空比计算方式 ------------------

extern uint8 key1_flag;
extern uint8 key2_flag;
extern uint8 key3_flag;
extern uint8 key4_flag;

extern float A_SPEED;
extern int32 Distance;
extern int32 g_encoder_raw;

extern float G_KP;
extern float G_KD;
extern float A_KP;
extern float N_SPEED;


extern int CTRL_flag;



void Buzzer_init(void);
void Buzzer_check(int TIME1);
void Key_init(void);
void Key_scan(void);
void key1_clear(void);
void key2_clear(void);
void key3_clear(void);
void key4_clear(void);


void CYT2_S_motor_ctrl(int32 SPEED);
void CYT2_S_motor_loop_ctrl(float T_SPEED);
void Motor_text(void);

void QUD_encoder_init(void);
void QUD_encoder_pulse_get(void);
float Cal_Distance(int32 A_SPEED);
float CYT2_get_distance_mag(int16 speed);
void Encoder_text(void);

void Steer_init(void);
void Steer_set(int angle);
void Steer_text(void);

void New_ctrl(void);
int16_t Remap_Angle_Linear(int16_t channel);
float KnobStepAngle(int16_t ch);
void GUN_ctrl_text(void);

//void Wx_paramt_init(void);
//void paramt_give(void);
//void new_ctrl(void);
#endif /* CODE_FOUNDATION_H_ */
