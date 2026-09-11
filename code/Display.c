/*
 * Diaplay.c
 *
 *  Created on: 2026年3月24日
 *      Author: zjx
 */


#include "zf_common_headfile.h"



int  func_index = 0; //初始显示
int  last_index = 127; //last初始为无效值


void (*current_operation_index)(void);       //显示函数索引指针(当前操作索引)

key_table table_dispaly[100]=                 //结构体数组
{
//{索引，向上，向下，确认，显示函数}
    //第0层
    {0,0,0,1,(*fun_0)},                     //AIIT_meun

    //第1层
    {1,6,2, 7,(*fun_a1)},
    {2,1,3,13,(*fun_b1)},
    {3,2,4,19,(*fun_c1)},
    {4,3,5,25,(*fun_d1)},
    {5,4,6,31,(*fun_e1)},
    {6,5,1, 0,(*fun_f1)},

    //第2层
    {7,12, 8, 37, (*fun_a21)},
    {8, 7, 9, 38, (*fun_a22)},
    {9, 8, 10,39, (*fun_a23)},
    {10,9, 11,40, (*fun_a24)},
    {11,10,12,41, (*fun_a25)},
    {12,11,7,  1, (*fun_a26)},            //ESC

    {13,18,14,42, (*fun_b21)},
    {14,13,15,43, (*fun_b22)},
    {15,14,16,44, (*fun_b23)},
    {16,15,17,45, (*fun_b24)},
    {17,16,18,46, (*fun_b25)},
    {18,17,13, 2, (*fun_b26)},           //ESC

    {19,24,20,47, (*fun_c21)},
    {20,19,21,48, (*fun_c22)},
    {21,20,22,49, (*fun_c23)},
    {22,21,23,50, (*fun_c24)},
    {23,22,24,51, (*fun_c25)},
    {24,23,19,3,  (*fun_c26)},           //ESC

    {25,30,26,52, (*fun_d21)},
    {26,25,27,53, (*fun_d22)},
    {27,26,28,54, (*fun_d23)},
    {28,27,29,55, (*fun_d24)},
    {29,28,30,56, (*fun_d25)},
    {30,29,25,4,  (*fun_d26)},           //ESC

    {31,36,32,57, (*fun_e21)},
    {32,31,33,58, (*fun_e22)},
    {33,32,34,59, (*fun_e23)},
    {34,33,35,60, (*fun_e24)},
    {35,34,36,61, (*fun_e25)},
    {36,35,31,5,  (*fun_e26)},           //ESC

    //第3层
    {37,37,37,7, (*fun_a31)},
    {38,38,38,8, (*fun_a32)},
    {39,39,39,9, (*fun_a33)},
    {40,40,40,10,(*fun_a34)},
    {41,41,41,11,(*fun_a35)},

    {42,42,42,13,(*fun_b31)},
    {43,43,43,14,(*fun_b32)},
    {44,44,44,15,(*fun_b33)},
    {45,45,45,16,(*fun_b34)},
    {46,46,46,17,(*fun_b35)},

    {47,47,47,19,(*fun_c31)},
    {48,48,48,20,(*fun_c32)},
    {49,49,49,21,(*fun_c33)},
    {50,50,50,22,(*fun_c34)},
    {51,51,51,23,(*fun_c35)},

    {52,52,52,25,(*fun_d31)},
    {53,53,53,26,(*fun_d32)},
    {54,54,54,27,(*fun_d33)},
    {55,55,55,28,(*fun_d34)},
    {56,56,56,29,(*fun_d35)},

    {57,57,57,31,(*fun_e31)},
    {58,58,58,32,(*fun_e32)},
    {59,59,59,33,(*fun_e33)},
    {60,60,60,34,(*fun_e34)},
    {61,61,61,35,(*fun_e35)},
};


void Menu(void)//菜单函数
{



                if(key1_flag)
                {

                    func_index = table_dispaly[func_index].up;    //向上翻
                    key1_clear();
                }
                if(key2_flag)
                {

                    func_index = table_dispaly[func_index].down;    //向下翻
                     key2_clear();

                }
                if(key3_flag)
                {

                    func_index = table_dispaly[func_index].enter;    //确认
                    key3_clear();

                }


            if (func_index != last_index)
            {
                current_operation_index = table_dispaly[func_index].current_operation;

                ips200_clear();
                (*current_operation_index)();//执行当前操作函数
                last_index = func_index;

            }
            else
            {
                (*current_operation_index)();//执行当前操作函数
            }
  }


///*********第0层***********/
void fun_0()
{


//    show_rgb565_image(0,16*5, (const uint16 *)gImage_ORRN, 240, 135, 240, 135, 0);
    ips200_show_string(0,250,"----------MISS---LY----------");


}

////////////////////////////////////////////////////////////////////////////////////////////////////////第一层///////////////////////////////////////////////////////////////////////////////////////////////////////////
void fun_a1()
{

    ips200_show_string(0,  16*1, "->");
    ips200_show_string(20, 16*1, "GPS");                 ips200_show_string(8*25, 16*15, "-1-");
    ips200_show_string(20, 16*2, "INS");
    ips200_show_string(20, 16*3, "GO");
    ips200_show_string(20, 16*4, "D");
    ips200_show_string(20, 16*5, "E");
    ips200_show_string(20, 16*6, "ESC");

}

void fun_b1()
{
    ips200_show_string(0,  16*2, "->");
    ips200_show_string(20, 16*1, "GPS");                 ips200_show_string(8*25, 16*15, "-1-");
    ips200_show_string(20, 16*2, "INS");
    ips200_show_string(20, 16*3, "GO");
    ips200_show_string(20, 16*4, "D");
    ips200_show_string(20, 16*5, "E");
    ips200_show_string(20, 16*6, "ESC");


}

void fun_c1()
{
    ips200_show_string(0,  16*3, "->");
    ips200_show_string(20, 16*1, "GPS");                 ips200_show_string(8*25, 16*15, "-1-");
    ips200_show_string(20, 16*2, "INS");
    ips200_show_string(20, 16*3, "GO");
    ips200_show_string(20, 16*4, "D");
    ips200_show_string(20, 16*5, "E");
    ips200_show_string(20, 16*6, "ESC");



}

void fun_d1()
{
    ips200_show_string(0,  16*4, "->");
    ips200_show_string(20, 16*1, "GPS");                 ips200_show_string(8*25, 16*15, "-1-");
    ips200_show_string(20, 16*2, "INS");
    ips200_show_string(20, 16*3, "GO");
    ips200_show_string(20, 16*4, "D");
    ips200_show_string(20, 16*5, "E");
    ips200_show_string(20, 16*6, "ESC");

}

void fun_e1()
{
    ips200_show_string(0,  16*5, "->");
    ips200_show_string(20, 16*1, "GPS");                 ips200_show_string(8*25, 16*15, "-1-");
    ips200_show_string(20, 16*2, "INS");
    ips200_show_string(20, 16*3, "GO");
    ips200_show_string(20, 16*4, "D");
    ips200_show_string(20, 16*5, "E");
    ips200_show_string(20, 16*6, "ESC");

}

void fun_f1()
{
    ips200_show_string(0,  16*6, "->");
    ips200_show_string(20, 16*1, "GPS");                 ips200_show_string(8*25, 16*15, "-1-");
    ips200_show_string(20, 16*2, "INS");
    ips200_show_string(20, 16*3, "GO");
    ips200_show_string(20, 16*4, "D");
    ips200_show_string(20, 16*5, "E");
    ips200_show_string(20, 16*6, "ESC");

}

////////////////////////////////////////////////////////////////////////////////////////////////////////第二层///////////////////////////////////////////////////////////////////////////////////////////////////////////
void fun_a21()//
{
    ips200_show_string(0,  16*1, "->");
    ips200_show_string(20, 16*1, "sub_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "sub_2");
    ips200_show_string(20, 16*3, "sub_3");
    ips200_show_string(20, 16*4, "sub_4");
    ips200_show_string(20, 16*5, "Force_erase");
    ips200_show_string(20, 16*6, "ESC");
}

void fun_a22()
{
    ips200_show_string(0,  16*2, "->");
    ips200_show_string(20, 16*1, "sub_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "sub_2");
    ips200_show_string(20, 16*3, "sub_3");
    ips200_show_string(20, 16*4, "sub_4");
    ips200_show_string(20, 16*5, "Force_erase");
    ips200_show_string(20, 16*6, "ESC");
}

void fun_a23()
{
    ips200_show_string(0,  16*3, "->");
    ips200_show_string(20, 16*1, "sub_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "sub_2");
    ips200_show_string(20, 16*3, "sub_3");
    ips200_show_string(20, 16*4, "sub_4");
    ips200_show_string(20, 16*5, "Force_erase");
    ips200_show_string(20, 16*6, "ESC");
}

void fun_a24()
{
    ips200_show_string(0,  16*4, "->");
    ips200_show_string(20, 16*1, "sub_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "sub_2");
    ips200_show_string(20, 16*3, "sub_3");
    ips200_show_string(20, 16*4, "sub_4");
    ips200_show_string(20, 16*5, "Force_erase");
    ips200_show_string(20, 16*6, "ESC");
}

void fun_a25()
{
    ips200_show_string(0,  16*5, "->");
    ips200_show_string(20, 16*1, "sub_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "sub_2");
    ips200_show_string(20, 16*3, "sub_3");
    ips200_show_string(20, 16*4, "sub_4");
    ips200_show_string(20, 16*5, "Force_erase");
    ips200_show_string(20, 16*6, "ESC");
}
void fun_a26()
{
    ips200_show_string(0,  16*6, "->");
    ips200_show_string(20, 16*1, "sub_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "sub_2");
    ips200_show_string(20, 16*3, "sub_3");
    ips200_show_string(20, 16*4, "sub_4");
    ips200_show_string(20, 16*5, "Force_erase");
    ips200_show_string(20, 16*6, "ESC");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void fun_b21()
{
    ips200_show_string(0,  16*1, "->");
    ips200_show_string(20, 16*1, "sub_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "sub_2");
    ips200_show_string(20, 16*3, "sub_3");
    ips200_show_string(20, 16*4, "B_4");
    ips200_show_string(20, 16*5, "sub_save)");
    ips200_show_string(20, 16*6, "ESC");
}

void fun_b22()
{
    ips200_show_string(0,  16*2, "->");
    ips200_show_string(20, 16*1, "sub_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "sub_2");
    ips200_show_string(20, 16*3, "sub_3");
    ips200_show_string(20, 16*4, "B_4");
    ips200_show_string(20, 16*5, "sub_save");
    ips200_show_string(20, 16*6, "ESC");
}

void fun_b23()
{
    ips200_show_string(0,  16*3, "->");
    ips200_show_string(20, 16*1, "sub_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "sub_2");
    ips200_show_string(20, 16*3, "sub_3");
    ips200_show_string(20, 16*4, "B_4");
    ips200_show_string(20, 16*5, "sub_save");
    ips200_show_string(20, 16*6, "ESC");
}

void fun_b24()
{
    ips200_show_string(0,  16*4, "->");
    ips200_show_string(20, 16*1, "sub_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "sub_2");
    ips200_show_string(20, 16*3, "sub_3");
    ips200_show_string(20, 16*4, "B_4");
    ips200_show_string(20, 16*5, "sub_save");
    ips200_show_string(20, 16*6, "ESC");
}

void fun_b25()
{
    ips200_show_string(0,  16*5, "->");
    ips200_show_string(20, 16*1, "sub_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "sub_2");
    ips200_show_string(20, 16*3, "sub_3");
    ips200_show_string(20, 16*4, "B_4");
    ips200_show_string(20, 16*5, "sub_save");
    ips200_show_string(20, 16*6, "ESC");
}

void fun_b26()
{
    ips200_show_string(0,  16*6, "->");
    ips200_show_string(20, 16*1, "sub_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "sub_2");
    ips200_show_string(20, 16*3, "sub_3");
    ips200_show_string(20, 16*4, "B_4");
    ips200_show_string(20, 16*5, "sub_save");
    ips200_show_string(20, 16*6, "ESC");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void fun_c21()
{
    ips200_show_string(0,  16*1, "->");
    ips200_show_string(20, 16*1, "GO_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "GO_2");
    ips200_show_string(20, 16*3, "GO_3");
    ips200_show_string(20, 16*4, "GO_4");
    ips200_show_string(20, 16*5, "C_5");
    ips200_show_string(20, 16*6, "ESC");
}

void fun_c22()
{
    ips200_show_string(0,  16*2, "->");
    ips200_show_string(20, 16*1, "GO_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "GO_2");
    ips200_show_string(20, 16*3, "GO_3");
    ips200_show_string(20, 16*4, "GO_4");
    ips200_show_string(20, 16*5, "C_5");
    ips200_show_string(20, 16*6, "ESC");
}

void fun_c23()
{
    ips200_show_string(0,  16*3, "->");
    ips200_show_string(20, 16*1, "GO_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "GO_2");
    ips200_show_string(20, 16*3, "GO_3");
    ips200_show_string(20, 16*4, "GO_4");
    ips200_show_string(20, 16*5, "C_5");
    ips200_show_string(20, 16*6, "ESC");
}

void fun_c24()
{
    ips200_show_string(0,  16*4, "->");
    ips200_show_string(20, 16*1, "GO_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "GO_2");
    ips200_show_string(20, 16*3, "GO_3");
    ips200_show_string(20, 16*4, "GO_4");
    ips200_show_string(20, 16*5, "C_5");
    ips200_show_string(20, 16*6, "ESC");
}

void fun_c25()
{
    ips200_show_string(0,  16*5, "->");
    ips200_show_string(20, 16*1, "GO_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "GO_2");
    ips200_show_string(20, 16*3, "GO_3");
    ips200_show_string(20, 16*4, "GO_4");
    ips200_show_string(20, 16*5, "C_5");
    ips200_show_string(20, 16*6, "ESC");
}

void fun_c26()
{
    ips200_show_string(0,  16*6, "->");
    ips200_show_string(20, 16*1, "GO_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "GO_2");
    ips200_show_string(20, 16*3, "GO_3");
    ips200_show_string(20, 16*4, "GO_4");
    ips200_show_string(20, 16*5, "C_5");
    ips200_show_string(20, 16*6, "ESC");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void fun_d21()
{
    ips200_show_string(0,  16*1, "->");
    ips200_show_string(20, 16*1, "D_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "D_2");
    ips200_show_string(20, 16*3, "D_3");
    ips200_show_string(20, 16*4, "D_4");
    ips200_show_string(20, 16*5, "D_5");
    ips200_show_string(20, 16*6, "ESC");
}

void fun_d22()
{
    ips200_show_string(0,  16*2, "->");
    ips200_show_string(20, 16*1, "D_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "D_2");
    ips200_show_string(20, 16*3, "D_3");
    ips200_show_string(20, 16*4, "D_4");
    ips200_show_string(20, 16*5, "D_5");
    ips200_show_string(20, 16*6, "ESC");
}

void fun_d23()
{
    ips200_show_string(0,  16*3, "->");
    ips200_show_string(20, 16*1, "D_1");                ips200_show_string(8*25, 16*15, "Page_2");
    ips200_show_string(20, 16*2, "D_2");
    ips200_show_string(20, 16*3, "D_3");
    ips200_show_string(20, 16*4, "D_4");
    ips200_show_string(20, 16*5, "D_5");
    ips200_show_string(20, 16*6, "ESC");
}

void fun_d24()
{
    ips200_show_string(0,  16*4, "->");
    ips200_show_string(20, 16*1, "D_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "D_2");
    ips200_show_string(20, 16*3, "D_3");
    ips200_show_string(20, 16*4, "D_4");
    ips200_show_string(20, 16*5, "D_5");
    ips200_show_string(20, 16*6, "ESC");
}

void fun_d25()
{
    ips200_show_string(0,  16*5, "->");
    ips200_show_string(20, 16*1, "D_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "D_2");
    ips200_show_string(20, 16*3, "D_3");
    ips200_show_string(20, 16*4, "D_4");
    ips200_show_string(20, 16*5, "D_5");
    ips200_show_string(20, 16*6, "ESC");
}

void fun_d26()
{
    ips200_show_string(0,  16*6, "->");
    ips200_show_string(20, 16*1, "D_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "D_2");
    ips200_show_string(20, 16*3, "D_3");
    ips200_show_string(20, 16*4, "D_4");
    ips200_show_string(20, 16*5, "D_5");
    ips200_show_string(20, 16*6, "ESC");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void fun_e21()
{
    ips200_show_string(0,  16*1, "->");
    ips200_show_string(20, 16*1, "Start_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "Start_2");
    ips200_show_string(20, 16*3, "Start_3");
    ips200_show_string(20, 16*4, "Start_4");
    ips200_show_string(20, 16*5, "E_5");
    ips200_show_string(20, 16*6, "ESC");
}

void fun_e22()
{
    ips200_show_string(0,  16*2, "->");
    ips200_show_string(20, 16*1, "Start_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "Start_2");
    ips200_show_string(20, 16*3, "Start_3");
    ips200_show_string(20, 16*4, "Start_4");
    ips200_show_string(20, 16*5, "E_5");
    ips200_show_string(20, 16*6, "ESC");
}

void fun_e23()
{
    ips200_show_string(0,  16*3, "->");
    ips200_show_string(20, 16*1, "Start_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "Start_2");
    ips200_show_string(20, 16*3, "Start_3");
    ips200_show_string(20, 16*4, "Start_4");
    ips200_show_string(20, 16*5, "E_5");
    ips200_show_string(20, 16*6, "ESC");
}

void fun_e24()
{
    ips200_show_string(0,  16*4, "->");
    ips200_show_string(20, 16*1, "Start_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "Start_2");
    ips200_show_string(20, 16*3, "Start_3");
    ips200_show_string(20, 16*4, "Start_4");
    ips200_show_string(20, 16*5, "E_5");
    ips200_show_string(20, 16*6, "ESC");
}

void fun_e25()
{
    ips200_show_string(0,  16*5, "->");
    ips200_show_string(20, 16*1, "Start_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "Start_2");
    ips200_show_string(20, 16*3, "Start_3");
    ips200_show_string(20, 16*4, "Start_4");
    ips200_show_string(20, 16*5, "E_5");
    ips200_show_string(20, 16*6, "ESC");
}

void fun_e26()
{
    ips200_show_string(0,  16*6, "->");
    ips200_show_string(20, 16*1, "Start_1");                ips200_show_string(8*25, 16*15, "-2-");
    ips200_show_string(20, 16*2, "Start_2");
    ips200_show_string(20, 16*3, "Start_3");
    ips200_show_string(20, 16*4, "Start_4");
    ips200_show_string(20, 16*5, "E_5");
    ips200_show_string(20, 16*6, "ESC");
}



////////////////////////////////////////////////////////////////////////////////////////////////////////第三层///////////////////////////////////////////////////////////////////////////////////////////////////////////

void fun_a31()//科目一相关
{

}

void fun_a32()//科目二相关
{

}



void fun_a33()//科目三
{

}

void fun_a34()//科目四
{

}


void fun_a35()//强制清除数据
{

}

void fun_b31()
{



}

void fun_b32()//SUB2 — 开始惯导录制
{
    if(gd_mode == guandao_pass_mode)
    {
        dis_record = 0.03;
        guandao_Init();
        gd_mode = guandao_record_mode;
        Mode_chage = 1;
        ips200_clear();
    }

    ips200_show_string(0,  16*0, ">>> REC SUB2 <<<");
    ips200_show_string(0,  16*1, "Points: ");
    ips200_show_int(8*10, 16*1, guandao_index, 5);
    ips200_show_string(0,  16*3, "Push the car");
    ips200_show_string(0,  16*4, "to draw figure-8");
    ips200_show_string(0,  16*6, "ENTER=back to menu");
}

void fun_b33()//SUB3 — 开始惯导录制
{
    if(gd_mode == guandao_pass_mode)
    {
        dis_record = 0.2;
        guandao_Init();
        gd_mode = guandao_record_mode;
        Mode_chage = 1;
        ips200_clear();
    }

    ips200_show_string(0,  16*0, ">>> REC SUB3 <<<");
    ips200_show_string(0,  16*1, "Points: ");
    ips200_show_int(8*10, 16*1, guandao_index, 5);
    ips200_show_string(0,  16*3, "Push the car");
    ips200_show_string(0,  16*4, "to draw figure-U");
    ips200_show_string(0,  16*6, "ENTER=back to menu");
}

void fun_b34()
{



}

void fun_b35()//保存录制
{
    if(gd_mode == guandao_record_mode)
    {
        guandao_flash_record();             // 保存到 Flash
        gd_mode = guandao_pass_mode;        // 回到空闲
        Mode_chage = 0;                     // 退出 Body_keep
        ips200_clear();
        Buzzer_check(100);                  // 滴一声提示
    }

    ips200_show_string(0,  16*0, ">>> SAVED <<<");
    ips200_show_string(0,  16*1, "Total points: ");
    ips200_show_int(8*16, 16*1, guandao_index, 5);
    ips200_show_string(0,  16*3, "ENTER=back to menu");
}

void fun_c31()
{
    SUB_flag=1;
}

void fun_c32()//GO_2 — 开始科目2（惯导回放）
{
    /* 如果还在录制状态，先自动保存再回放 */
    if(gd_mode == guandao_record_mode)
    {
        guandao_flash_record();
        gd_mode = guandao_pass_mode;
        Buzzer_check(100);
        ips200_clear();
    }

    /* 空闲 → 尝试从 Flash 加载路径 */
//    Motor_Standard_Speed = 383;               // 科目2
      Motor_Standard_Speed = 350;               // 科目2-----------------------------------------------------*********
    dis_record = 0.03;                      // 科目2：3cm
    if(gd_mode == guandao_pass_mode)
    {
        guandao_flash_load();
        if(guandao_index > 0)
        {
            data_x.f = 0; data_y.f = 0;
            guandao_lucheng = 0;
            c_error = 0; int_c_error = 0;
            guandao_last_cnt = 0; guandao_new_cnt = 0;
            gd_mode = guandao_load_mode;
            SUB_flag = 2;
            step = 2;             //----------------步进幅度-值越大，转向越灵敏
            stl_kp = 8;           //----------------横向偏差-值越大，束缚越强
            ips200_clear();
        }
    }

    ips200_show_string(0,  16*0, ">>> RUN SUB2 <<<");
    ips200_show_string(0,  16*1, "Status: ");
    ips200_show_int(8*10, 16*1, gd_mode, 2);
    ips200_show_string(0,  16*2, "Points: ");
    ips200_show_int(8*10, 16*2, guandao_index, 5);
    ips200_show_string(0,  16*4, "ENTER=back to menu");
}

void fun_c33()//GO_3 — 开始科目3（惯导+弯道减速）
{
    if(gd_mode == guandao_record_mode)
    {
        guandao_flash_record();
        gd_mode = guandao_pass_mode;
        Buzzer_check(100);
        ips200_clear();
    }

    Motor_Standard_Speed = 640;               // 科目3  62  630
    dis_record = 0.2;                      // 科目3：20cm
    if(gd_mode == guandao_pass_mode)
    {
        guandao_flash_load();
        if(guandao_index > 0)
        {
            data_x.f = 0; data_y.f = 0;
            guandao_lucheng = 0;
            c_error = 0; int_c_error = 0;
            guandao_last_cnt = 0; guandao_new_cnt = 0;
            gd_mode = guandao_load_mode;
            SUB_flag = 3;
            step = 11;
            stl_kp = 18;
            ips200_clear();
        }
    }

    ips200_show_string(0,  16*0, ">>> RUN SUB3 <<<");
    ips200_show_string(0,  16*1, "Status: ");
    ips200_show_int(8*10, 16*1, gd_mode, 2);
    ips200_show_string(0,  16*2, "Points: ");
    ips200_show_int(8*10, 16*2, guandao_index, 5);
    ips200_show_string(0,  16*4, "ENTER=back to menu");
}

void fun_c34()
{


}


void fun_c35()
{


}

void fun_d31()
{


}

void fun_d32()
{


}

void fun_d33()
{


}

void fun_d34()
{


}


void fun_d35()
{


}

void fun_e31()//科目一
{


}

void fun_e32()//科目二
{

}

void fun_e33()//科目三
{

}

void fun_e34()
{


}


void fun_e35()
{


}


int SUB_flag=0;
void Sub_select(int sub)//科目选择
{
    if(sub==1)
    {
        Body_ctrl_1();
    }
    if(sub==2)
    {
        Body_ctrl_2();
    }
    if(sub==3)
    {
        Body_ctrl_3();
    }



}
