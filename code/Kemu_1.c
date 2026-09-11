/*
 * Kemu_1.c
 *
 *  Created on: 2026年4月3日
 *      Author: 赵佳鑫
 */

#include "zf_common_headfile.h"

uint32 Mode_chage=0;
uint32 sys_times=0;
float  End_error=0;
float  Taget_angle=0;


void Body_ctrl_1(void)
{



    static int32 I=0;

    I++;
    static bool once = false;     //保证只赋值一次

        if (!once)
        {
          Taget_angle=0;              //起始角度为0
          balance_mode_parameter(1);   //分配科目一参数
          once = true;
        }

    if(I>=6500)
    {
        Taget_angle=180;
      //  gpio_set_level(Buzzer_pin,1);
    }

    static float last_gyro_y = 0.0f;



        imu660ra_gyro_x=LowPassFilter(imu660ra_gyro_x, last_gyro_y, 0.1f);
        last_gyro_y=imu660ra_gyro_x;

    if(sys_times%1==0)//角速度环
    {

                pid_control(&roll_balance_cascade.angular_speed_cycle, roll_balance_cascade.angle_cycle.out, imu660ra_gyro_x);


    }

    if(sys_times%5==0)//角度环
    {

          pid_control(&roll_balance_cascade.angle_cycle, roll_balance_cascade.turn_cycle.out-roll_balance_cascade.posture_value.mechanical_zero, -roll_balance_cascade.posture_value.rol);

    }

    if(sys_times%20==0)//转向环
    {

                pid_control(&roll_balance_cascade.turn_cycle, AngleErrorNormalize(StepApproach(Taget_angle,1.5)-roll_balance_cascade.posture_value.yaw),0);



    }


         Steer_set(SERVO_MOTOR_MID +roll_balance_cascade.angular_speed_cycle.out);

     // CYT2_S_motor_ctrl(2000);

         //if(sys_times % 5 == 0)
        // {
         //    CYT2_S_motor_loop_ctrl(200);
         //}
         //CYT2_S_motor_loop_ctrl(1000);


         if(I < 5500)
         {
             CYT2_S_motor_loop_ctrl(1000);               // 0~5.5s：闭环 1000 RPM
         }
         else if(I < 6500)
         {
             CYT2_S_motor_loop_ctrl(3035 - I * 37 / 100); // 5.5~6.5s：1000 线性降到 630
         }
         else if(I < 8000)
         {
             CYT2_S_motor_loop_ctrl(630);                 // 6.5~8s：保持 630
         }
         else if(I < 10000)
         {
             CYT2_S_motor_loop_ctrl(I * 37 / 200 - 850); // 8~10s：630 → 1000
         }
         else
         {
             CYT2_S_motor_loop_ctrl(1000);                // 8s 之后：恢复 1000
         }
//--------------------------------------------------------------------------------------------
//         if(I < 6000)
//         {
//             CYT2_S_motor_ctrl(2000);               // 0~6s：全速
//         }
//         else if(I < 7000)
//         {
//             CYT2_S_motor_ctrl(5500 - I / 2);       // 6~7s：2000 线性降到 1500
//         }
//         else if(I < 8500)
//         {
//             CYT2_S_motor_ctrl(1500);               // 7~8.5s：保持 1500
//         }
//         else
//         {
//             CYT2_S_motor_ctrl(2000);               // 8.5s 之后：恢复全速
 //        }
}

void Body_keep(void)//只依靠角速度和角度维持车头随动
{
    static int32 I=0;   //系统计时
    I++;


    static bool once = false;     //保证只赋值一次
    if (!once)
    {
      balance_mode_parameter(1);   //分配科目一参数

      once = true;
    }


    Imu_lowpass_filter();       //IMU数据一阶低通滤波



    if(sys_times%1==0)//角速度环
    {

        if(Imu_type==1)//在选择IMU型号的时候就已经确定
        {
            pid_control(&roll_balance_cascade.angular_speed_cycle, roll_balance_cascade.angle_cycle.out, imu660ra_gyro_x);
        }
        else if(Imu_type==2)
        {
            pid_control(&roll_balance_cascade.angular_speed_cycle, roll_balance_cascade.angle_cycle.out, imu660rb_gyro_y);
        }
        else if(Imu_type==3)
        {
            pid_control(&roll_balance_cascade.angular_speed_cycle, roll_balance_cascade.angle_cycle.out, imu963ra_gyro_y);
        }

    }

    if(sys_times%5==0)//角度环
    {
            pid_control(&roll_balance_cascade.angle_cycle, 0-roll_balance_cascade.posture_value.mechanical_zero, -roll_balance_cascade.posture_value.rol);

//          pid_control(&roll_balance_cascade.angle_cycle, roll_balance_cascade.turn_cycle.out-roll_balance_cascade.posture_value.mechanical_zero, -roll_balance_cascade.posture_value.rol);

    }



         Steer_set(SERVO_MOTOR_MID +roll_balance_cascade.angular_speed_cycle.out);

}


void Balance_1_text(void)
{
    ips_show_string(8*0, 16*16, "");
    ips_show_string(8*0, 16*17, "YAW:");      ips_show_float(8*10,16*17, roll_balance_cascade.posture_value.yaw,3,6);
    ips_show_string(8*0, 16*18, "ROLL:");     ips_show_float(8*10,16*18, roll_balance_cascade.posture_value.rol,3,6);
    ips_show_string(8*0, 16*19,  "------------");       ips_show_float(8*13,16*19, guandao_lucheng,3,6);
    ips_show_string(8*17, 16*19, "------------");

}
