/*
 * guandao.c
 *
 *  Created on: 2026年5月13日
 *      Author:  labu
 */

#include "zf_common_headfile.h"

float Motor_Standard_Speed = 33.0f;         //目标车速
float step = 8.0;//2




//变量
#define GD_PI       3.1415926f
#define GD_DEG2RAD  (GD_PI / 180.0f)

guandao_mode gd_mode = guandao_pass_mode;

union FloatInspector data_x,data_y,data_yaw;  //记录当前点

float dis_record = 0.03;//科目1和科目3需要改成0.2-20CM

int guandao_index = 0;         //记录点的目录
int guandao_cnt = 0;           //当前循迹点
int guandao_last_cnt = 0;
int guandao_new_cnt = 0;
int forward = 5;               //前瞻长度（原2→3，提前转向，弯道更准）
int window = 10;               //搜点窗口

float guandao_lucheng = 0;       //记录走了多少距离
float X_Record_f[MAX] = {0};      //读取得到的x的数组
float Y_Record_f[MAX] = {0};      //读取得到的y的数组
float Yaw_Record_f[MAX] = {0};    //读取得到的yaw数组
uint32 X_Record_i[MAX] = {0};       //要上传的x的数组
uint32 Y_Record_i[MAX] = {0};       //要上传的y的数组
uint32 Yaw_Record_i[MAX] = {0};     //要上传的yaw数组

float theta0 = 0;    //theta0   记录当前偏航角    输出目标值偏航角
float theta1 = 0;    //theta1   flash中的偏航角
float theta2 = 0;    //theta2   坐标计算得出的偏航角
float c_error = 0;
float int_c_error = 0, int_c_max = 40, int_c_min = -40;
float stl_kp = 15;//8 //8.5
float stl_ki = 2000;    //2000
/*思路实现：
 * 1.记录每一个点的yaw
 * 2.记录每一个点的x，y坐标
 * 3.读取下一个点的x，y坐标与当前x，y坐标连线，得出转向目标值
 */

//工具函数
float gougu(float x1,float y1,float x2,float y2)
{
    float dx = x1 - x2;
    float dy = y1 - y2;
    return sqrt(dx * dx + dy * dy);
}

/*
 * x,y是当前坐标
 * X_Record_f,Y_Record_f是路径坐标数组
 * len是数组长度
 * per_cnt是上一次匹配的索引
 */
float cross_error(float x , float y , float *X_Record_f , float *Y_Record_f , int len , int pre_cnt , int *new_cnt)    //横向误差计算
{
    int start = pre_cnt - window;
    int end = pre_cnt + window;

    if (start < 0) start = 0;               //限定索引范围
    if (end >= len) end = len - 1;

    int best_cnt = pre_cnt;
    float min_dist2 = INFINITY;

    for (int i = start; i <= end; ++i)
    {
        float dx = x - X_Record_f[i];
        float dy = y - Y_Record_f[i];
        float d2 = dx*dx + dy*dy;
        if (d2 < min_dist2)
        {
            min_dist2 = d2;
            best_cnt = i;
        }
    }

    *new_cnt = best_cnt + forward;

    // 用匹配点的切线方向计算横向误差
    float theta = Yaw_Record_f[best_cnt] * 3.1415926f / 180.0f;
    float dx = x - X_Record_f[best_cnt];
    float dy = y - Y_Record_f[best_cnt];
    float cross_track = dy * cosf(theta) - dx * sinf(theta);

    return cross_track;
}

//实现函数

//记录坐标
void guandao_record(void)
{
    if(guandao_lucheng <= dis_record)
    {
        return;
    }

    guandao_lucheng -= dis_record;

    if(gd_mode == guandao_record_mode)
    {
        X_Record_i[guandao_index] = data_x.i;
        Y_Record_i[guandao_index] = data_y.i;
        Yaw_Record_i[guandao_index] = data_yaw.i;

        guandao_index ++;
    }
}

//上传坐标
void guandao_flash_record(void)
{
    uint16 lenh,lenx,leny,lenyaw;
    uint32 *ph,*px,*py,*pyaw;

    lenh = 1;
    lenx = MAX;
    leny = MAX;
    lenyaw = MAX;
    ph = (uint32)&guandao_index;
    px = X_Record_i;
    py = Y_Record_i;
    pyaw = Yaw_Record_i;

    flash_erase_page(0, FLASH_H_PAGE);
    flash_erase_page(0, FLASH_X_PAGE);
    flash_erase_page(0, FLASH_Y_PAGE);
    flash_erase_page(0, FLASH_YAW_PAGE);

    flash_write_page(0, FLASH_H_PAGE, ph, lenh);
    flash_write_page(0, FLASH_X_PAGE, px, lenx);
    flash_write_page(0, FLASH_Y_PAGE, py, leny);
    flash_write_page(0, FLASH_YAW_PAGE, pyaw, lenyaw);
}

//读取坐标,并转化为float供循迹
void guandao_flash_load(void)
{
    uint16 lenh,lenx,leny,lenyaw;
    uint32 *ph,*px,*py,*pyaw;
    int i;

    lenh = 1;
    lenx = MAX;
    leny = MAX;
    lenyaw = MAX;
    ph = (uint32)&guandao_index;
    px = X_Record_i;
    py = Y_Record_i;
    pyaw = Yaw_Record_i;

    flash_read_page(0, FLASH_H_PAGE, ph, lenh);
    flash_read_page(0, FLASH_X_PAGE, px, lenx);
    flash_read_page(0, FLASH_Y_PAGE, py, leny);
    flash_read_page(0, FLASH_YAW_PAGE, pyaw, lenyaw);

    for(i = 0; i < guandao_index; i++)
    {
        data_x.i = X_Record_i[i];
        X_Record_f[i] = data_x.f;
        data_y.i = Y_Record_i[i];
        Y_Record_f[i] = data_y.f;
        data_yaw.i = Yaw_Record_i[i];
        Yaw_Record_f[i] = data_yaw.f;


 //打印每个索引的数据
        printf("INDEX %d: X=%f, Y=%f, YAW=%f\r\n", i, X_Record_f[i], Y_Record_f[i], Yaw_Record_f[i]);

        printf("(%f,%f)\r\n",X_Record_f[i], Y_Record_f[i]);


    }

}


float T_A=0;
void guandao_load(void)
{
    if(guandao_lucheng <= dis_record)
    {
        return;
    }

    guandao_lucheng -= dis_record;

    c_error = cross_error(data_x.f , data_y.f , X_Record_f , Y_Record_f , guandao_index , guandao_last_cnt , &guandao_new_cnt);//算出横向误差同时更新当前跑的点

    int_c_error += stl_ki * c_error * 0.001;//0.001和中断没关系是个常量
    //int_c_error *= 0.99f;

    if(int_c_error >= int_c_max){int_c_error = int_c_max;}
    if(int_c_error <= int_c_min){int_c_error = int_c_min;}

    /* 取前瞻点的 yaw 作为 Stanley 航向参考（用 guandao_new_cnt = best_cnt + 2） */
    int yaw_ref_idx = guandao_new_cnt;
    if(yaw_ref_idx >= guandao_index) { yaw_ref_idx = guandao_index - 1; }

    float temp = Yaw_Record_f[yaw_ref_idx] - atanf(stl_kp * c_error / Motor_Standard_Speed)*180.0f / 3.1415926f - int_c_error;//temp-目标角度
//    float current_speed = CYT2_get_distance_mag(motor_value.receive_left_speed_data) * 1000.0f;
//    if(current_speed < 0.5f) current_speed = 0.5f;
//    float temp = Yaw_Record_f[yaw_ref_idx] - atanf(stl_kp * c_error / current_speed)*180.0f / 3.1415926f - int_c_error;

    temp = AngleErrorNormalize(temp);


//    printf("temp=%f\r\n",temp);

    float diff = AngleErrorNormalize(temp - Taget_angle);


    if(fabsf(diff) <= step)
       {
        Taget_angle = temp;
       }
       else if(diff > 0.0f)
       {
           Taget_angle = AngleErrorNormalize(Taget_angle+ step);
       }
       else
       {
           Taget_angle = AngleErrorNormalize(Taget_angle - step);
       }
//    printf("T_A=%f\r\n",T_A);
    guandao_last_cnt = guandao_new_cnt;
}

//if(guandao_new_cnt>=-INDEX)
//{
//  点读完了
//}

//初始化函数
//全部初始化
void guandao_Init(void)
{
    memset(&X_Record_f, 0, sizeof(X_Record_f));
    memset(&Y_Record_f, 0, sizeof(Y_Record_f));

    memset(&Yaw_Record_f, 0, sizeof(Yaw_Record_f));
    memset(&X_Record_i, 0, sizeof(X_Record_i));

    memset(&Y_Record_i, 0, sizeof(Y_Record_i));
    memset(&Yaw_Record_i, 0, sizeof(Yaw_Record_i));

    guandao_lucheng = 0;
    guandao_index = 0;
    guandao_cnt = 0;
    guandao_last_cnt = 0;
    guandao_new_cnt = 0;

    data_x.f = 0;
    data_y.f = 0;

    gd_mode = guandao_pass_mode;    //默认为空闲状态
}

void guandao_task(void)
{
    if(gd_mode == guandao_record_mode)
    {
        guandao_record();
    }
    else if(gd_mode == guandao_load_mode)
    {
        guandao_load();
    }
}

void Ins_text(void)
{
        if(key1_flag==1)//开始录制
        {
            key1_flag=0;

            Buzzer_check(50);
            gpio_set_level(LED1,0);

            /* 当前空闲 -> 开始录制 */
            if(gd_mode == guandao_pass_mode)
            {
                /* 开始录制航向序列 */
                gd_mode = guandao_record_mode;

                Mode_chage=1;//开启车头随动
            }

        }

        if(key2_flag==1)//到终点停止录制并保存
        {
            key2_flag=0;
            Buzzer_check(100);
            gpio_set_level(LED2,0);

            if(gd_mode == guandao_record_mode)
            {
                /* 保存录制结果到 Flash */
                   guandao_flash_record();

                   gd_mode = guandao_pass_mode;
            }

        }

        if(key3_flag==1)//复现路径
        {
            key3_flag=0;
            Buzzer_check(300);
            gpio_set_level(LED3,0);


            if(gd_mode == guandao_pass_mode)
            {
                /* 从 Flash 中读取录好的航向序列，并开始回放 */
                guandao_flash_load();

                /* 重置惯导位置和积分项，消除录制→回放之间的累积漂移 */
                data_x.f = 0;
                data_y.f = 0;
                guandao_lucheng = 0;
                c_error = 0;
                int_c_error = 0;
                guandao_last_cnt = 0;
                guandao_new_cnt = 0;

                gd_mode = guandao_load_mode;


                Mode_chage=2;



            }




        }




}


void INS_log(void)
{
    ips_show_string(8*0, 16*0, "Distance:");        ips_show_float(8*10,16*0, guandao_lucheng,3,6);
    ips_show_string(8*0, 16*1, "YAW:");             ips_show_float(8*10,16*1, theta0,3,6);
    ips_show_string(8*0, 16*2, "X:");               ips_show_float(8*10,16*2, data_x.f ,3,6);
    ips_show_string(8*0, 16*3, "Y");                ips_show_float(8*10,16*3, data_y.f ,3,6);
    ips_show_string(8*0, 16*4, "D_YAW");            ips_show_float(8*10,16*4, data_yaw.f,3,6);
    ips_show_string(8*0, 16*5, "INDEX");            ips_show_uint(8*10,16*5, guandao_index,5);
    ips_show_string(8*0, 16*6, "T_A");              ips_show_float(8*10,16*6, Taget_angle,3,6);

}


void INS_data_get(void)
{
    // float d = Cal_Distance(g_encoder_raw);              // 编码器测距（旧）
    float d = CYT2_get_distance_mag(motor_value.receive_left_speed_data); // 磁编测距（新）

    guandao_lucheng += d;                               //积分距离

    theta0 = roll_balance_cascade.posture_value.yaw;    //获取偏航角
    data_x.f += d * cosf(theta0 * 3.1415926f / 180.0f); //单位距离的X轴分位移
    data_y.f += d * sinf(theta0 * 3.1415926f / 180.0f); //单位距离的Y轴分位移
    data_yaw.f = theta0;
}
