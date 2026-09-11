/*
 * Angle_control.c
 *
 *  Created on: 2026年4月1日
 *      Author: zjx
 */
#include "zf_common_headfile.h"


/**
 * @brief  一阶低通滤波器（增量式，纯函数）
 * @param  current  当前采样值
 * @param  last     上一次滤波输出值
 * @param  alpha    滤波系数 [0, 1]，越小越平滑，越大响应越快
 * @return          本次滤波输出值
 * @note   首次使用时，last 应与 current 相同以完成初始化
 *         示例：filtered = LowPassFilter(raw, filtered, 0.2f);
 */
float LowPassFilter(float current, float last, float alpha)
{
    return last + alpha * (current - last);
}

/**
 * @brief  目标值步进逼近（纯函数，供直接嵌入调用）
 * @param  target     最终目标值
 * @param  step_size  每次步进量（正值）
 * @return            本次步进后的值（内部用 static 保存状态）
 */
float StepApproach(float target, float step_size)
{
    static float current = 0.0f;

    if (current < target)
    {
        current += step_size;
        if (current > target)
            {
            current = target;
            }
    } else if (current > target)
    {
        current -= step_size;
        if (current < target)
            {
            current = target;
            }
    }
    return current;
}



/**
 * @brief  将角度误差归一化到 [-180, 180] 范围内
 * @param  error  原始角度误差（度）
 * @return        归一化后的角度误差
 * @note   用于解决 yaw 角在 ±180° 边界跳变导致 PID 误差爆炸的问题
 *         示例：angle_error = AngleErrorNormalize(target_yaw - current_yaw);
 */
float AngleErrorNormalize(float error)
{
    if (error > 180.0f)
        return error - 360.0f;
    else if (error < -180.0f)
        return error + 360.0f;

    else
        return error;
}

/**
 * @brief  角度步进逼近（支持最短路径和强制方向，自动处理±180°边界）
 * @param  target       最终目标角度（度）
 * @param  real_angle   当前实际角度（度，用于初始化）
 * @param  step_size    每次步进最大变化量（度，>0）
 * @param  mode         0=自动最短路径，1=强制顺时针（长路径），-1=强制逆时针（长路径）
 * @return              步进后的目标角度（度，归一化到 [-180,180]）
 * @note   首次调用时会用 real_angle 初始化内部状态
 */
float StepApproachAngleMode(float target, float real_angle, float step_size, int mode)
{
    static float current = 0.0f;
    static uint8_t init = 0;

    if (!init) {
        current = real_angle;
        init = 1;
    }

    float err;  // 当前输出值到目标值的误差（带符号，绝对值表示需转动的角度）

    if (mode == 0) {
        // 最短路径：取最小角度差，范围 [-180,180]
        err = target - current;
        if (err > 180.0f) err -= 360.0f;
        else if (err < -180.0f) err += 360.0f;
    }
    else if (mode == 1) {
        // 强制顺时针：误差 = -顺时针需转的角度（0~360）
        float cw = fmodf(current - target, 360.0f);
        if (cw < 0) cw += 360.0f;
        err = -cw;   // 顺时针为负
    }
    else { // mode == -1
        // 强制逆时针：误差 = +逆时针需转的角度（0~360）
        float ccw = fmodf(target - current, 360.0f);
        if (ccw < 0) ccw += 360.0f;
        err = ccw;   // 逆时针为正
    }

    // 步进限制
    if (fabsf(err) <= step_size) {
        current = target;
    } else {
        current += (err > 0 ? step_size : -step_size);
    }

    // 归一化输出到 [-180, 180]
    if (current > 180.0f) current -= 360.0f;
    if (current < -180.0f) current += 360.0f;

    return current;
}
