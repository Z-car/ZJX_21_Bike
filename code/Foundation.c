/*
 * Foundation.c
 *
 *  Created on: 2026年1月26日
 *      Author: zjx
 */

#include "zf_common_headfile.h"

#define wheel_diameter  (0.07f)     // 轮子直径（单位 米）

//--------------------------------------------------------------------------蜂鸣器-通用

void Buzzer_init(void)
{
    gpio_init(Buzzer_pin, GPO, 0, GPO_PUSH_PULL);
}

void Buzzer_check(int TIME1)
{
    gpio_set_level(Buzzer_pin,1);//响
    system_delay_ms(TIME1);
    gpio_set_level(Buzzer_pin,0);//不响
}

//--------------------------------------------------------------------------按键-通用

uint8 key1_state = 1;                                                               // 按键动作状态
uint8 key2_state = 1;                                                               // 按键动作状态
uint8 key3_state = 1;                                                               // 按键动作状态
uint8 key4_state = 1;                                                               // 按键动作状态

uint8 key1_state_last = 0;                                                          // 上一次按键动作状态
uint8 key2_state_last = 0;                                                          // 上一次按键动作状态
uint8 key3_state_last = 0;                                                          // 上一次按键动作状态
uint8 key4_state_last = 0;                                                          // 上一次按键动作状态

uint8 key1_flag;
uint8 key2_flag;
uint8 key3_flag;
uint8 key4_flag;


void Key_init(void)
{
    gpio_init(KEY1, GPI, 1, GPI_PULL_UP);
    gpio_init(KEY2, GPI, 1, GPI_PULL_UP);
    gpio_init(KEY3, GPI, 1, GPI_PULL_UP);
    gpio_init(KEY4, GPI, 1, GPI_PULL_UP);

    gpio_init(LED1, GPO, 1, GPO_PUSH_PULL);          // 初始化 LED1 输出 默认高电平 推挽输出模式
    gpio_init(LED2, GPO, 1, GPO_PUSH_PULL);         // 初始化 LED2 输出 默认高电平 推挽输出模式
    gpio_init(LED3, GPO, 1, GPO_PUSH_PULL);          // 初始化 LED3 输出 默认高电平 推挽输出模式
    gpio_init(LED4, GPO, 1, GPO_PUSH_PULL);         // 初始化 LED4 输出 默认高电平 推挽输出模式

    gpio_init(Switch1, GPI, 1, GPI_FLOATING_IN);
    gpio_init(Switch2, GPI, 1, GPI_FLOATING_IN);

}

void Key_scan(void)
{
    //保存当前按键状态
    key1_state_last = key1_state;
    key2_state_last = key2_state;
    key3_state_last = key3_state;
    key4_state_last = key4_state;

    //更新当前按键状态
    key1_state = gpio_get_level(KEY1);
    key2_state = gpio_get_level(KEY2);
    key3_state = gpio_get_level(KEY3);
    key4_state = gpio_get_level(KEY4);

    //比较状态
    if(key1_state && !key1_state_last)   {key1_flag = 1;}
    if(key2_state && !key2_state_last)   {key2_flag = 1;}
    if(key3_state && !key3_state_last)   {key3_flag = 1;}
    if(key4_state && !key4_state_last)   {key4_flag = 1;}
}

void key1_clear(void)
{
  key1_flag=0;
  Buzzer_check(50);

}

void key2_clear(void)
{
  key2_flag=0;
  Buzzer_check(50);

}

void key3_clear(void)
{
  key3_flag=0;
  Buzzer_check(50);

}

void key4_clear(void)
{
  key4_flag=0;
  Buzzer_check(50);

}

//--------------------------------------------------------------------------无刷电机-CYT2-

void CYT2_S_motor_ctrl(int32 SPEED)
{
    SPEED=SPEED>M_MAX?M_MAX:(SPEED<M_MIN)?M_MIN:SPEED;//限幅

    small_driver_set_duty((int16)SPEED,0);
}


//void CYT2_S_motor_loop_ctrl(float T_SPEED)
//{
//    pid_control(&roll_balance_cascade.speed_cycle,T_SPEED ,-motor_value.receive_left_speed_data );
 //   CYT2_S_motor_ctrl(roll_balance_cascade.speed_cycle.out);
//}
void CYT2_S_motor_loop_ctrl(float T_SPEED)
{
    static float filtered_speed = 0;
    static uint8 init = 0;
    if(!init) { filtered_speed = (float)(-motor_value.receive_left_speed_data); init = 1; }

    float raw = (float)(-motor_value.receive_left_speed_data);
    filtered_speed += 0.3f * (raw - filtered_speed);

    pid_control(&roll_balance_cascade.speed_cycle, T_SPEED, filtered_speed);

    float out = roll_balance_cascade.speed_cycle.out;
    if(out < 150)   out = 150;       // 最小油门，不断油
    if(out > M_MAX) out = M_MAX;     // 上限 5000

    CYT2_S_motor_ctrl((int32)out);
}

void Motor_text(void)//电机测试
{

       static int32 S_PSEED=0;

       Key_scan();

          if(key1_flag)
             {
                 key1_flag=0;
                 S_PSEED+=100;
             }
          if(key2_flag)
             {
                 key2_flag=0;
                 S_PSEED-=100;
             }
          if(key3_flag)
             {
                 key3_flag=0;
                 S_PSEED+=500;
             }
          if(key4_flag)
             {
                 key4_flag=0;
                 S_PSEED-=500;
             }


          ips_show_string(8*0,16*1, "Motor_text");

          ips_show_string(8*0,16*3, "V:");          ips_show_int(8*5,  16*3,S_PSEED, 5);


          CYT2_S_motor_ctrl(S_PSEED);
}


//--------------------------------------------------------------------------正交编码器-
float A_SPEED=0;
int32 Distance=0;
void QUD_encoder_init(void)
{
//    encoder_quad_init(ENCODER_1, ENCODER_1_A, ENCODER_1_B);                     // 初始化编码器模块与引脚 正交解码编码器模式
    encoder_dir_init(ENCODER_1, ENCODER_1_A, ENCODER_1_B);
}

int32 g_encoder_raw = 0;   // 原始（未滤波）编码器计数，供惯导距离积分用

void QUD_encoder_pulse_get(void)
{
//    static float last_A_SPEED= 0.0f;
//
//    g_encoder_raw = encoder_get_count(ENCODER_1);                     // 保存原始计数（未滤波）
//
//    A_SPEED = LowPassFilter((float)g_encoder_raw, last_A_SPEED, 0.1f);
//    last_A_SPEED=A_SPEED;

//    encoder_clear_count(ENCODER_1);                                             // 清空编码器计数
    A_SPEED=-motor_value.receive_left_speed_data;
}

float Cal_Distance(int32 A_SPEED)
{
    float wheel_c   = 3.1415926f * 0.070;               // 轮周长---0.07-N车轮子直径
    float wheel_rev = ((float)A_SPEED) / 1024 / 4;      //1024-编码器线数    4-齿轮减速比
    return wheel_rev * wheel_c;
}

// 磁编测距（1ms中断调用，单路磁编，形参同 Cal_Distance，返回单次里程增量 单位米）
float CYT2_get_distance_mag(int16 speed)
{
    float speed_f = -(float)speed;                                                  // 磁编单路速度（RPM）
    return (speed_f / 60.0f * wheel_diameter * PI * 0.001f);                        // 返回单次里程  单位米
}

void Encoder_text(void)//电机测试
{

       static int32 S_PSEED=0;

       Key_scan();

          if(key1_flag)//左电机+500
             {
                 key1_flag=0;
                 S_PSEED+=50;
             }
          if(key2_flag)//左电机-500
             {
                 key2_flag=0;
                 S_PSEED-=50;
             }
          if(key3_flag)//右电机+500
             {
                 key3_flag=0;
                 S_PSEED+=500;
             }
          if(key4_flag)//右电机-500
             {
                 key4_flag=0;
                 S_PSEED-=500;
             }


          ips_show_string(8*0,16*1, "Encoder_text");

          ips_show_string(8*0,16*3, "V:");          ips_show_int(8*7,  16*3,S_PSEED, 5);
          ips_show_string(8*0,16*4, "E_V:");        ips_show_int(8*7,  16*4,A_SPEED,5);
          ips_show_string(8*0,16*5, "B_V:");        ips_show_int(8*7,  16*5,-motor_value.receive_left_speed_data,5);
          ips_show_string(8*0,16*6, "D:");          ips200_show_float(8*7,  16*6,guandao_lucheng,5,5);


          CYT2_S_motor_ctrl(S_PSEED);
}


//--------------------------------------------------------------------------舵机

void Steer_init(void)//舵机初始化
{
    pwm_init(SERVO_MOTOR_PWM, SERVO_MOTOR_FREQ, (uint32)SERVO_MOTOR_DUTY(SERVO_MOTOR_MID));
}


void Steer_set(int angle)//舵机驱动
{
    if(angle>SERVO_MOTOR_RMAX){angle=SERVO_MOTOR_RMAX;}//限幅
    if(angle<SERVO_MOTOR_LMAX){angle=SERVO_MOTOR_LMAX;}
    pwm_set_duty(SERVO_MOTOR_PWM, (uint32)SERVO_MOTOR_DUTY(angle));
}

void Steer_text(void)//舵机测试
{

       static int32 angle=SERVO_MOTOR_MID;

       Key_scan();
          if(key1_flag)
             {
                 key1_flag=0;
                 angle+=10;
             }
          if(key2_flag)
             {
                 key2_flag=0;
                 angle-=10;
             }
          if(key3_flag)
             {
                 key3_flag=0;
                 angle+=1;
    //             angle=55;//左打死
             }
          if(key4_flag)
             {
                 key4_flag=0;
                angle-=1;
    //             angle=85;//右打死
             }


          ips_show_string(0,100, "Steer_text");
          ips_show_int(100,  16*3,angle, 5);
          Steer_set(angle);
}


//--------------------------------------------------------------------------遥控器

int CTRL_flag=0;

/**
 * @brief  遥控器通道值线性映射到角度 -180° ~ 180°
 * @param  channel  原始通道值 (192~1777, 中值 992)
 * @return 角度值，范围 -180 ~ 180
 */
int16_t Remap_Angle_Linear(int16_t channel)
{
    const int16_t CH_MIN   = 192;
        const int16_t CH_MAX   = 1777;
        const int16_t CH_MID   = 992;
        const int16_t ANG_MAX  = 360;          // 最大角度改为 360
        const int16_t DEAD_ZONE = 15;          // 死区宽度（通道值）

        if(channel < CH_MIN) channel = CH_MIN;
        if(channel > CH_MAX) channel = CH_MAX;

        if(channel > CH_MID - DEAD_ZONE && channel < CH_MID + DEAD_ZONE)
            return 0;

        float angle;
        if(channel <= CH_MID - DEAD_ZONE)
        {
            // 左转区域：通道值从 CH_MIN 到 CH_MID-DEAD_ZONE，映射到 -ANG_MAX 到 0
            int16_t left_max = CH_MID - DEAD_ZONE;
            angle = -ANG_MAX + (channel - CH_MIN) * (ANG_MAX / (float)(left_max - CH_MIN));
        }
        else
        {
            // 右转区域：通道值从 CH_MID+DEAD_ZONE 到 CH_MAX，映射到 0 到 ANG_MAX
            int16_t right_min = CH_MID + DEAD_ZONE;
            angle = (channel - right_min) * (ANG_MAX / (float)(CH_MAX - right_min));
        }

        if(angle > ANG_MAX) angle = ANG_MAX;
        if(angle < -ANG_MAX) angle = -ANG_MAX;

        return (int16_t)angle;
}

/**
 * @brief   旋钮步进角度选择：每次拧动并回中后，切换到下一个预设角度
 * @param   ch   当前通道值 (192~1777)
 * @return  当前目标角度
 */
float KnobStepAngle(int16_t ch)
{
//    static const float angles[] = {45.0f, -180.0f,135.0f,180.0f};
    static const float angles[] = {180};

    static uint8_t idx = 0;                 // 下一个要输出的角度索引（初始指向 -45）
    static uint8_t state = 0;               // 0: 等待拧动, 1: 已经拧出等待回中
    static float current_angle = 0.0f;      // 当前目标角度（初始0）

    const int16_t MID = 992;
    const int16_t DEAD = 30;                // 中点死区 (±30)
    const int16_t EXIT = 200;                // 离开中点的最小偏移（确认是有效拧动）

    // 限制通道范围
    if (ch < 192) ch = 192;
    if (ch > 1777) ch = 1777;

    int16_t offset = ch - MID;
    uint8_t is_mid = (offset > -DEAD && offset < DEAD);
    uint8_t is_exit = (offset > EXIT || offset < -EXIT);

    if (state == 0) {
        // 等待第一次拧出
        if (!is_mid && is_exit) {
            state = 1;      // 标记已经拧出
        }
    } else if (state == 1) {
        // 已经拧出，等待回中
        if (is_mid) {
            // 回中完成：切换角度
            current_angle = angles[idx];
            idx = (idx + 1) % 4;   // 指向下一个
            state = 0;             // 复位，准备下一次
        }
    }

    return current_angle;
}

;
void New_ctrl(void)
{
    if(1 == uart_receiver.finsh_flag)                            // 帧完成标志判断
           {
               if(1 == uart_receiver.state)                             // 遥控器失控状态判断
               {
                   //舵机控制
//                          if(uart_receiver.channel[0]>=1500&&uart_receiver.channel[0]<=1792)
//                          {
//                              Taget_angle=180;
////                              Taget_angle-=3;
//                          }
//
//                           else if(uart_receiver.channel[0]>=192&&uart_receiver.channel[0]<=700)
//                           {
//                               Taget_angle-=45;
////                               Taget_angle+=3;
//                           }
//                           else
//                           {
//                               Taget_angle=0;
//                           }

//                   Taget_angle=KnobStepAngle(uart_receiver.channel[0]);
//                   printf("%f\r\n",Taget_angle);

                          //电机控制
                          if(uart_receiver.channel[2]>=1500)
                          {
                              CYT2_S_motor_ctrl(0);
                              Steer_set(SERVO_MOTOR_MID);

                          }
                         else
                          {
                             CYT2_S_motor_ctrl(1500);
                             CTRL_flag=0;
                          }
               }
               else
               {
                   printf("Remote control has been disconnected.\r\n"); // 串口输出失控提示
               }
               uart_receiver.finsh_flag = 0;                            // 帧完成标志复位
           }

}

void GUN_ctrl_text(void)
{
    if(1 == uart_receiver.finsh_flag)                            // 帧完成标志判断
    {
        if(1 == uart_receiver.state)                             // 遥控器失控状态判断
        {
            printf("CH1-CH6 data: ");
            for(int i = 0; i < 6; i++)
            {
                printf("%d ", uart_receiver.channel[i]);         // 串口输出6个通道数据
            }
            printf("\r\n");
        }
        else
        {
            printf("Remote control has been disconnected.\r\n"); // 串口输出失控提示
        }
        uart_receiver.finsh_flag = 0;                            // 帧完成标志复位
    }
}

////--------------------------------------------------------------------------无线串口
//
//void Wx_paramt_init(void)
//{
//    wireless_uart_init();
//    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_WIRELESS_UART);
//}
//
//float G_KP=0;
//float G_KD=0;
//float A_KP=0;
//float N_SPEED=0;
//void paramt_give(void)
//{
//    seekfree_assistant_data_analysis();
//
//    B_G_PID.Kp=seekfree_assistant_parameter[0];
//    B_G_PID.Kd=seekfree_assistant_parameter[1];
//    B_A_PID.Kp=seekfree_assistant_parameter[2];
//    B_A_PID.Kd=seekfree_assistant_parameter[3];
//    N_SPEED=   seekfree_assistant_parameter[4];
//
//    ips200_show_float(0,16*0,seekfree_assistant_parameter[0],3,5);
//    ips200_show_float(0,16*1,seekfree_assistant_parameter[1],3,5);
//    ips200_show_float(0,16*2,seekfree_assistant_parameter[2],3,5);
//    ips200_show_float(0,16*3,seekfree_assistant_parameter[3],3,5);
//    ips200_show_float(0,16*4,seekfree_assistant_parameter[4],4,5);
//
//}
//
//int CTRL_flag=0;
//void new_ctrl(void)
//{
//    if(1 == uart_receiver.finsh_flag)                            // 帧完成标志判断
//           {
//               if(1 == uart_receiver.state)                             // 遥控器失控状态判断
//               {
////                   //舵机控制
////                          if(uart_receiver.channel[0]>=1500&&uart_receiver.channel[0]<=1792)
////                          {
////                              Steer_set(SERVO_MOTOR_LMAX);
////                          }
////
////                           else if(uart_receiver.channel[0]>=192&&uart_receiver.channel[0]<=700)
////                           {
////                               Steer_set(SERVO_MOTOR_RMAX);
////
////                           }
////                           else
////                           {
////                               Steer_set(SERVO_MOTOR_MID);
////                           }
//
//
//                          //电机控制
//                          if(uart_receiver.channel[2]>=1500)
//                          {
//                              CYT2_S_motor_ctrl(2000+abs(sys_para.eulerAngle.pitch*50));
//                              CTRL_flag=0;
//                          }
//                           else
//                           {
//                               CYT2_S_motor_ctrl(0);
//                               Steer_set(SERVO_MOTOR_MID);
//                               CTRL_flag=1;
//                           }
//               }
//               else
//               {
//                   printf("Remote control has been disconnected.\r\n"); // 串口输出失控提示
//               }
//               uart_receiver.finsh_flag = 0;                            // 帧完成标志复位
//           }
//
//}
