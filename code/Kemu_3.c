/*
 *
 * Kemu_3.c
 *
 *  Created on: 2026年6月5日
 *      Author: 赵佳鑫
 *
 */

#include "zf_common_headfile.h"



void Body_ctrl_3(void)
{
    static float speed_target = 300;           //改
    static bool ramp_done = false;           //改

    static bool once = false;
    if (!once)
    {
      balance_mode_parameter(3);            // 科目3参数（有角速度环）

      Taget_angle = roll_balance_cascade.posture_value.yaw;
      once = true;
    }


    Imu_lowpass_filter();                   // IMU数据一阶低通滤波



    /* ===== 3环串级控制（同科目1） ===== */

    if(sys_times%1==0)//角速度环
    {
        if(Imu_type==1)
        {
            pid_control(&roll_balance_cascade.angular_speed_cycle, roll_balance_cascade.angle_cycle.out, imu660ra_gyro_x);
        }

    }

    if(sys_times%5==0)//角度环
    {
        pid_control(&roll_balance_cascade.angle_cycle, roll_balance_cascade.turn_cycle.out-roll_balance_cascade.posture_value.mechanical_zero, -roll_balance_cascade.posture_value.rol);
    }

    if(sys_times%20==0)//转向环 — Taget_angle 由 INS 更新
    {
        pid_control(&roll_balance_cascade.turn_cycle, AngleErrorNormalize(Taget_angle - roll_balance_cascade.posture_value.yaw), 0);
    }


     Steer_set(SERVO_MOTOR_MID + roll_balance_cascade.angular_speed_cycle.out);

     /* 路径终点检测，自动停车 */
     if(guandao_new_cnt >= guandao_index)
     {
         CYT2_S_motor_loop_ctrl(0);
     }
     else
     {
         //CYT2_S_motor_loop_ctrl(Motor_Standard_Speed);//闭环速度
/*         if(sys_times % 10 == 0)
         {
             CYT2_S_motor_loop_ctrl(630);
         }*/

         float progress = (float)guandao_new_cnt / (float)guandao_index;   // 0.0 ~ 1.0

                  if(!ramp_done)
                  {
                      speed_target += 20;
                      if(speed_target >= 640) { speed_target = 640; ramp_done = true; }
                  }

                  if(progress > 0.85f)    // 最后 20% 路程提速
                          {
                              speed_target = 1000;   // 提多少改这个值
                          }

                  if(sys_times % 5 == 0)
                  {
                      CYT2_S_motor_loop_ctrl(speed_target);
                  }
         //CYT2_S_motor_loop_ctrl(1200);
         //CYT2_S_motor_ctrl(1000);//开环速度
     }


}
