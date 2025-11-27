#include <cstdlib>
// #include <algorithm>
#include "ws_clock.h"
static uint32_t clock_color = Color(0, 1, 0);  //灯泡亮度和颜色控制
//设置一个像素时间以4x2 显示
// static uint8_t clock_high = 4;   //设置像素时间高度为4
// static uint8_t clock_wight = 2;  //设置像素时间宽度为2
Adafruit_NeoMatrix Matrix = Adafruit_NeoMatrix(8, 8, RGB_Control_PIN,
                                               NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
                                               NEO_RGB + NEO_KHZ800);
static int MatrixWidth = 0;
String clock_time;

void Matrix_Init() {
  Matrix.begin();
  Matrix.setTextWrap(false);
  Matrix.setBrightness(40);
  MatrixWidth = Matrix.width();
}
static uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}
void rotal_Leds(uint8_t dir) {
  matrix_copy();  // 拷贝数组到show_data

  while (dir--) rotateSquareMatrix();  //这是注释部分代码的简写  update by gpt
}
static int rotatetemp;
static int layer, first, last, i, j;
static void rotateSquareMatrix() {  //向右旋转一次

  for (uint8_t row = 0; row < 8; row++) {  // update by gpt
    for (uint8_t col = 0; col < 8; col++) {
      rotation_tmp[col][7 - row] = Show_Data[row][col];  // 顺时针旋转 90°
    }
  }
  memcpy(Show_Data, rotation_tmp, sizeof(Show_Data));
}
static int RGB_Leds_row, RGB_Leds_col;
void RGB_Leds() {
  for (RGB_Leds_row = 0; RGB_Leds_row < 8; RGB_Leds_row++) {
    for (RGB_Leds_col = 0; RGB_Leds_col < 8; RGB_Leds_col++) {

      Matrix.drawPixel(RGB_Leds_col, RGB_Leds_row, Show_Data[RGB_Leds_row][RGB_Leds_col]);
    }
  }
  Matrix.show();
}
void set_wifistatus_ok() {  //wifi ok 显示绿色
  // printf("color green :%d", color);
  Real_Data[0][0] = Color(0, 1, 0);
}
void set_wifistatus_err() {  //wifi err 显示红色
  // printf("color red :%d", color);
  Real_Data[0][0] = Color(1, 0, 0);
}
void set_ntp_ok() {  //wifi ok 显示绿色
  Real_Data[0][1] = Color(0, 1, 0);
}
void set_ntp_err() {  //wifi err 显示红色

  Real_Data[0][1] = Color(1, 0, 0);
}
static uint8_t matrix_copy_row, matrix_copy_col;
static void matrix_copy() {
  for (matrix_copy_row = 0; matrix_copy_row < 8; matrix_copy_row++) {
    for (matrix_copy_col = 0; matrix_copy_col < 8; matrix_copy_col++) {
      Show_Data[matrix_copy_row][matrix_copy_col] = Real_Data[matrix_copy_row][matrix_copy_col];
    }
  }
}

void set_Clock(uint8_t t_hour, uint8_t t_min, uint8_t t_sec) {  //12:34:56

  set_hour(t_hour);
  set_min(t_min);
  set_sec(t_sec);
}

static void set_hour(uint8_t hour) {
  ;
  set_num_display(binary_pixel(hour), Hour_OFFSET);  //不同单位所在的位置不同
}
static void set_min(uint8_t min) {

  set_num_display(binary_pixel(min), MIN_OFFSET);
}

static void set_sec(uint8_t sec) {

  set_num_display(binary_pixel(sec), SEC_OFFSET);
}
static int set_sec_col, set_sec_row;
static void set_num_display(bool time[][CLOCK_WIGHT], uint8_t offset) {
  for (set_sec_col = 1; set_sec_col >= 0; set_sec_col--) {
    for (set_sec_row = CLOCK_HIGH - 1; set_sec_row >= 0; set_sec_row--) {
      if (time[set_sec_row][set_sec_col] == false) {  //这个点位不需要亮
        Real_Data[set_sec_row + Matrix_Row - CLOCK_HIGH][set_sec_col + offset] = 0;
      } else {
        Real_Data[set_sec_row + Matrix_Row - CLOCK_HIGH][set_sec_col + offset] = clock_color;
      }
    }
  }
}


// -  8
// -  4
// 32 2
// 16 1
uint8_t binary_pixel_last_index;
static bool (* binary_pixel(uint8_t num))[2] {
  static bool time[CLOCK_HIGH][CLOCK_WIGHT]; 
  memset(time, 0, sizeof(time));  // 一次性清零

    // 右列（从上到下）：8, 4, 2, 1 对应 time[0..3][1]
  time[0][1] = (num & 0x08) != 0; // 8
  time[1][1] = (num & 0x04) != 0; // 4
  time[2][1] = (num & 0x02) != 0; // 2
  time[3][1] = (num & 0x01) != 0; // 1

  // 左列：row2 = 32, row3 = 16
  time[2][0] = (num & 0x20) != 0; // 32
  time[3][0] = (num & 0x10) != 0; // 16
  return time;
}
