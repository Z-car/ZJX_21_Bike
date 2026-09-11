
/*
 * guandao.h
 *
 *  Created on: 2026年5月13日
 *      Author: labu
 */

#ifndef CODE_GUANDAO_H_
#define CODE_GUANDAO_H_

#define dis_position 0.01       //计算位置距离
#define MAX 1000         //最多存点个数
#define FLASH_H_PAGE 8   //存头部参数
#define FLASH_X_PAGE 9   //存x的页数
#define FLASH_Y_PAGE 10  //存y的页数
#define FLASH_YAW_PAGE 11  //存y的页数

typedef enum
{
    guandao_pass_mode = 0,     // 空闲
    guandao_record_mode,       // 录制航向
    guandao_load_mode          // 回放循迹
} guandao_mode;

union FloatInspector {
    float    f;   // 4 字节
    uint32   i;   // 4 字节
};

extern guandao_mode gd_mode;
extern union FloatInspector data_x,data_y,data_yaw;

extern float dis_record;
extern int guandao_index;         //记录点的目录
extern int guandao_cnt;           //当前循迹点
extern int guandao_last_cnt;
extern int guandao_new_cnt;
extern float guandao_lucheng;
extern float X_Record_f[MAX];
extern float Y_Record_f[MAX];
extern float Yaw_Record_f[MAX];
extern uint32 Yaw_Record_i[MAX];

extern float Motor_Standard_Speed;

extern float theta0;    //theta0   记录当前偏航角    输出目标值偏航角
extern float theta1;    //theta1   flash中的偏航角
extern float theta2;    //theta2   坐标计算得出的偏航角
extern float c_error;
extern float int_c_error;
extern float stl_kp;    //8.5
extern float stl_ki;
extern float step;

void guandao_record(void);
void guandao_flash_record(void);
void guandao_flash_load(void);
void guandao_load(void);
void guandao_Init(void);
void guandao_task(void);

void Ins_text(void);
void INS_log(void);
void INS_data_get(void);

#endif /* CODE_GUANDAO_H_ */
