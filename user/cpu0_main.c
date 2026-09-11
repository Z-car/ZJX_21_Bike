
#include "zf_common_headfile.h"
#pragma section all "cpu0_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中


// **************************** 代码区域 ****************************
int core0_main(void)
{
    clock_init();                   // 获取时钟频率<务必保留>
    debug_init();                   // 初始化默认调试串口
    seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_DEBUG_UART);//---------------------用示波器时打开  有线

 //   wireless_uart_init();                                                  // 新增：初始化无线串口
 //   seekfree_assistant_interface_init(SEEKFREE_ASSISTANT_WIRELESS_UART);   // 改：无线！

 //   Buzzer_init();                                      //蜂鸣器初始化
    Key_init();                                         //按键
    Steer_init();                                       //舵机
    small_driver_uart_init();                           //无刷驱动
    QUD_encoder_init();                                 //编码器
    ips_init(IPS200_TYPE_SPI);                          //显屏
    Imu_init();                                         //IMU
////
    balance_cascade_init();                             //平衡及陀螺仪参数初始化
    guandao_Init();                                     //惯导初始化

    Buzzer_check(50);                                       //外设初始化成功

    system_delay_ms(1000);

    pit_ms_init(CCU60_CH0,1);


    Buzzer_check(300);                                       //定时器初始化成功


    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready();         // 等待所有核心初始化完毕
    while (TRUE)
    {
        // 此处编写需要循环执行的代码
//        Motor_text();
//        Encoder_text();
//        Steer_text();
//        IMU_text();
        Balance_1_text();

        /* 虚拟示波器：每10ms发一次，放主循环不发在ISR里 */

//        static uint32 scope_tick = 0;
 //       if(++scope_tick >= 10)
 //       {
  //          scope_tick = 0;
  //          seekfree_assistant_oscilloscope_struct s;
  //          s.data[0] = roll_balance_cascade.turn_cycle.out - roll_balance_cascade.posture_value.mechanical_zero;  // 目标roll
   //         s.data[1] = -roll_balance_cascade.posture_value.rol;   // 实际roll
   //        s.data[2] = roll_balance_cascade.angle_cycle.out;       // 角度环输出（直接给舵机）


 //           s.data[0] = AngleErrorNormalize(Taget_angle - roll_balance_cascade.posture_value.yaw);  // CH1: yaw误差
 //           s.data[1] = roll_balance_cascade.turn_cycle.out;    // CH2: 转向环输出
 //           s.data[2] = c_error;                                 // CH3: 横向误差(m)
 //           s.channel_num = 3;
 //           seekfree_assistant_oscilloscope_send(&s);
  //      }
//        static uint32 scope_tick = 0;
//               if(++scope_tick >= 10)
 //              {
 //                  scope_tick = 0;
 //                  seekfree_assistant_oscilloscope_struct s;

  //                 s.data[0] = (float)(-motor_value.receive_left_speed_data);                     // CH1: 实际速度
  //                 s.data[1] = 630.0f;                                                             // CH2: 目标速度
  //                 s.data[2] = roll_balance_cascade.speed_cycle.p *
  //                             roll_balance_cascade.speed_cycle.p_value_last;                      // CH3: P项
  //                 s.data[3] = roll_balance_cascade.speed_cycle.i *
  //                             roll_balance_cascade.speed_cycle.i_value;                           // CH4: I项
  //                 s.data[4] = (float)roll_balance_cascade.speed_cycle.out;                       // CH5: PID总输出

  //                 s.channel_num = 5;
  //                 seekfree_assistant_oscilloscope_send(&s);
  //                 system_delay_ms(1);
    //           }//科目三速度环


/*  测零飘         static uint32 scope_tick = 0;
           if(++scope_tick >= 50)
           {
               scope_tick = 0;
               seekfree_assistant_oscilloscope_struct imu_scope;
              // imu_scope.data[0] =(float)imu660ra_gyro_x;//-------------------------真实值
               //imu_scope.data[1] =(float)imu660ra_gyro_y;//-------------------------真实值
               //imu_scope.data[2] =(float)imu660ra_gyro_z;//-------------------------真实值
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////三通道-----------分开测
               imu_scope.data[0] = roll_balance_cascade.posture_value.rol;//-------------------------拟合值
               imu_scope.data[1] = roll_balance_cascade.posture_value.pit;//-------------------------拟合值
               imu_scope.data[2] = roll_balance_cascade.posture_value.yaw;//-------------------------拟合值
               imu_scope.channel_num = 3;
               seekfree_assistant_oscilloscope_send(&imu_scope);
           }
           system_delay_ms(1); */  // ← 加这行，不然循环太快串口来不及发


//        Ins_text();

        /* 惯导回放时显示完整跟踪数据，否则显示普通 INS 日志 */
//        if(Mode_chage == 2)
//        {
//            Balance_2_text();       // LCD 显示惯导跟踪调试数据
//            Balance_2_printf();     // 串口输出惯导跟踪数据
//        }
//        else
//        {
//            INS_log();
//        }

//        New_ctrl();

        Menu();


//        printf("%d,%f\r\n",-motor_value.receive_left_speed_data,roll_balance_cascade.speed_cycle.out);
//         printf("%d,%f\r\n",-motor_value.receive_left_speed_data,guandao_lucheng);

//        CYT2_S_motor_ctrl(1);
        // 此处编写需要循环执行的代码
    }
}


IFX_INTERRUPT(cc60_pit_ch0_isr, 0, CCU6_0_CH0_ISR_PRIORITY)
{
    interrupt_global_enable(0);                        // 开启中断嵌套
    pit_clear_flag(CCU60_CH0);

    sys_times ++;                                      // 系统计时自增

    if(sys_times%5==0)
    {
        Key_scan();                                    //按键扫描
    }

    Imu_attitude_scan();                               //姿态解算

    //QUD_encoder_pulse_get();                           //编码器数据采集

    INS_data_get();                                  //惯导数据采集

    guandao_task();                                  //惯导读取与复现





    if(CTRL_flag==0)//默认无遥控器执行正常程序
    {
        Sub_select(SUB_flag);
    }


    if(Mode_chage==1)//车头随动模式用以惯导推车
    {
        Body_keep();
    }


}



#pragma section all restore
// **************************** 代码区域 ****************************
