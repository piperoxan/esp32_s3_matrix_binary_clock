#include <cstdlib>
// #include <algorithm>
#include "ws_clock.h"
static uint16_t clock_color = Color(0, 2, 0);  //灯泡亮度和颜色控制
Adafruit_NeoMatrix Matrix = Adafruit_NeoMatrix(8, 8, RGB_Control_PIN,
                                               NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
                                               NEO_RGB + NEO_KHZ800);
static int MatrixWidth = 0;
String clock_time;

void Matrix_Init() {
  Matrix.begin();
  Matrix.setTextWrap(false);
  Matrix.setBrightness(40);
  Matrix.setTextColor(Matrix.Color(0, 30, 0));
  MatrixWidth = Matrix.width();
}
static uint32_t Color(uint8_t r, uint8_t g, uint8_t b) {
  return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b;
}
void rotal_Leds(Direction dir) {
  matrix_copy();  // 拷贝数组到show_data
  switch (dir) {
    case UP:
      // std::copy(std::begin(Real_Data), std::end(Real_Data), std::begin(Show_Data)); // 拷贝数组
      break;
    case RIGHT:
      rotateSquareMatrix();
      break;
    case DOWN:
      rotateSquareMatrix();
      rotateSquareMatrix();
      break;
    case LEFT:
      rotateSquareMatrix();
      rotateSquareMatrix();
      rotateSquareMatrix();
      break;
  }
}
static int rotatetemp;
static int layer, first, last, i, j;
static void rotateSquareMatrix() {  //向右旋转一次
  // uint8_t color[3] = { 0 };
  for (layer = 0; layer < 8 / 2; layer++) {
    first = layer;
    last = 8 - 1 - layer;

    for (i = first; i < last; i++) {
      // 保存左上角的值
      rotatetemp = Show_Data[first][i];
      Show_Data[first][i] = Show_Data[last - (i - first)][first];
      Show_Data[last - (i - first)][first] = Show_Data[last][last - (i - first)];
      Show_Data[last][last - (i - first)] = Show_Data[i][last];
      Show_Data[i][last] = rotatetemp;
    }
  }
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
  uint16_t color = Color(0, 10, 0);
  // printf("color green :%d", color);
  Real_Data[0][0] = color;
}
void set_wifistatus_err() {  //wifi err 显示红色
  uint16_t color = Color(10, 0, 0);
  // printf("color red :%d", color);
  Real_Data[0][0] = color;
}
void set_ntp_ok() {  //wifi ok 显示绿色
  uint16_t color = Color(0, 10, 0);
  Real_Data[0][1] = color;
}
void set_ntp_err() {  //wifi err 显示红色
  uint16_t color = Color(10, 0, 0);
  Real_Data[0][1] = color;
}
static uint8_t matrix_copy_row, matrix_copy_col;
static void matrix_copy() {
  for (matrix_copy_row = 0; matrix_copy_row < 8; matrix_copy_row++) {
    for (matrix_copy_col = 0; matrix_copy_col < 8; matrix_copy_col++) {
      Show_Data[matrix_copy_row][matrix_copy_col] = Real_Data[matrix_copy_row][matrix_copy_col];
    }
  }
}
//将时间赋予时钟系统
void set_time(String ttime) {
  clock_time = ttime;
}
static uint8_t clock_hour, clock_min, clock_sec;
void set_Clock() {  //12:34:56
  // uint8_t hour, min, sec;
  // printf("timestring: %s\n", clock_time);
  clock_hour = std::atoi(clock_time.substring(0, 2).c_str());
  clock_min = std::atoi(clock_time.substring(3, 5).c_str());
  clock_sec = std::atoi(clock_time.substring(6, 8).c_str());
  // printf("h:%d,min:%d,sec:%d\n", clock_hour, clock_min, clock_sec);
  set_hour(clock_hour);
  set_min(clock_min);
  set_sec(clock_sec);

  // set_hour(1);
  // set_min(2);
  // set_sec(3);

  // delay(1000);
  // RGB_Leds();
}
// void fake_time() {
//   Matrix.drawPixel(1, 1, 10);
//   Matrix.show();
// }

static int set_hour_col, set_hour_row;
static void set_hour(uint8_t hour) {
  binary_pixel(hour, Hour);

  for (set_hour_col = 1; set_hour_col >= 0; set_hour_col--) {
    for (set_hour_row = 4; set_hour_row >= 0; set_hour_row--) {
      if (Hour[set_hour_row][set_hour_col] == false) {  //这个点位不需要亮
        Real_Data[set_hour_row + 3][set_hour_col] = 0;
        // printf("row:%d , col: %d\n", set_hour_row, set_hour_col);
      } else {
        Real_Data[set_hour_row + 3][set_hour_col] = clock_color;
      }
    }
  }
}
static int set_min_col, set_min_row;
static void set_min(uint8_t min) {
  binary_pixel(min, Min);
  for (set_min_col = 1; set_min_col >= 0; set_min_col--) {
    for (set_min_row = 4; set_min_row >= 0; set_min_row--) {
      if (Min[set_min_row][set_min_col] == false) {  //这个点位不需要亮
        Real_Data[set_min_row + 3][set_min_col + 3] = 0;
      } else {
        Real_Data[set_min_row + 3][set_min_col + 3] = clock_color;
      }
    }
  }
}
static int set_sec_col, set_sec_row;
static void set_sec(uint8_t sec) {
  binary_pixel(sec, Sec);
  for (set_sec_col = 1; set_sec_col >= 0; set_sec_col--) {
    for (set_sec_row = 4; set_sec_row >= 0; set_sec_row--) {
      if (Sec[set_sec_row][set_sec_col] == false) {  //这个点位不需要亮
        Real_Data[set_sec_row + 3][set_sec_col + 6] = 0;
      } else {
        Real_Data[set_sec_row + 3][set_sec_col + 6] = clock_color;
      }
    }
  }
}

// 16
// 8
// 4
// 2
// 1
uint8_t binary_pixel_last_index;
static void binary_pixel(uint8_t num, bool time[][2]) {
  binary_pixel_last_index = 5;
  // std::fill(time,time+10,false);//重置单元为false
  time[0][0] = false;
  time[1][0] = false;
  time[2][0] = false;
  time[3][0] = false;
  time[4][0] = false;
  time[0][1] = false;
  time[1][1] = false;
  time[2][1] = false;
  time[3][1] = false;
  time[4][1] = false;
  if (num > 31) {  //左侧那一列
    time[4][0] = true;
    num = num - 32;
  } else {
    time[4][0] = false;
  }

  for (; binary_pixel_last_index > 0; binary_pixel_last_index--) {
    // printf("binary_pixel:for_num- %d\n", num);
    if (num % 2 == 1) {
      time[binary_pixel_last_index - 1][1] = true;

    } else {
      time[binary_pixel_last_index - 1][1] = false;
    }
    num = num / 2;

    // printf("-------------\n");
  }
}
