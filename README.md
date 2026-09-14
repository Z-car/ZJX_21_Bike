# ADS_ZJX_bike_project

基于英飞凌 AURIX TC264 的智能车工程（自平衡单车）。

## 代码结构

```
                ┌──────────────────────────────┐
                │     cpu0_main.c  （主程序）    │
                │   初始化所有外设 + 主循环调度   │
                └──────────────┬───────────────┘
                               │ 调用
      ┌──────────┬────────────┼────────────┬────────────┐
      │          │            │            │            │
┌─────▼─────┐┌───▼────┐┌──────▼─────┐┌─────▼─────┐┌─────▼─────┐
│  平衡控制  ││ 惯导解算 ││  角度控制   ││  屏幕显示  ││  科目任务  │
│ balance   ││ guandao ││   Angle    ││  Display  ││ Kemu1/2/3 │
└─────┬─────┘└───┬────┘└────────────┘└───────────┘└───────────┘
      │          │
┌─────▼─────┐┌───▼────┐
│ 陀螺仪控制 ││  IMU   │
│Gyro_ctrl ││ (驱动)  │
└───────────┘└────────┘
```

## 模块说明

| 文件 | 作用 |
|------|------|
| `user/cpu0_main.c` | 主程序：初始化所有外设 + 主循环 |
| `code/Gyro_control.c/h` | 陀螺仪 / 姿态核心控制 |
| `code/guandao.c/h` | 惯导解算（姿态融合） |
| `code/Angle_control.c/h` | 角度控制 |
| `code/Display.c/h` | 屏幕显示 |
| `code/Foundation.c/h` | 基础 / 公共函数 |
| `code/Kemu_1/2/3.c/h` | 科目任务（比赛赛道） |
| `code/gyro.h` | 陀螺仪接口 |

## 目录结构

```
ZJX_ADS_Bike/
├── code/                 用户核心代码
│   ├── Gyro_control.c/h  陀螺仪 / 姿态控制
│   ├── guandao.c/h       惯导解算
│   ├── Angle_control.c/h 角度控制
│   ├── Display.c/h       屏幕显示
│   ├── Foundation.c/h    基础函数
│   ├── Kemu_1/2/3.c/h    科目任务
│   └── gyro.h            陀螺仪接口
├── user/                 主程序
│   ├── cpu0_main.c       核心 0 主程序
│   └── cpu1_main.c       核心 1 主程序
├── libraries/            驱动库（第三方）
│   ├── zf_*              逐飞科技库
│   └── infineon_libraries 英飞凌 iLLD 底层驱动
└── .cproject / .project  ADS 工程文件
```
