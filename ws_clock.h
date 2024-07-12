#include <algorithm>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoMatrix.h>
#include <utility>
#include "stdio.h"
#include "ws_grav.h"
#define RGB_Control_PIN 14
#define Matrix_Row 8
#define Matrix_Col 8
#define RGB_COUNT 64

// struct LED{
//   uint8_t color[3];
// };
static uint16_t Show_Data[8][8], Real_Data[8][8]; //定义信息显示, show data 为旋转之后, real data 为旋转前
static uint32_t Color(uint8_t r, uint8_t g, uint8_t b); //定义颜色方法(从adafruit pixel 中搞过来的)
static bool Hour[5][2];  //时间区块:小时对应点位是否点亮
static bool Min[5][2]; //时间区块:分钟对应点位是否点亮
static bool Sec[5][2]; //时间区块:秒钟对应点位是否点亮
void matrix_copy();  //复制read data 到show data , 准备显示
void RGB_Leds();  //点亮led 点阵
void Matrix_Init(); //初始化led 点阵
void fake_time();  //假显示, 用于测试
void rotal_Leds(Direction dir); //旋转 show data
void set_wifistatus_ok(); 
void set_wifistatus_err();
void set_ntp_ok();
void set_ntp_err();
void rotateSquareMatrix(); //向右旋转一次show data 数据
void set_hour(uint8_t hour); 
void set_min(uint8_t min);
void set_sec(uint8_t sec);
void set_time(String time); //传递ntp 时间到 clock
void set_Clock(); 
void binary_pixel(uint8_t num, bool time[][2]); //将时间数字转化为二进制写入对应数组
