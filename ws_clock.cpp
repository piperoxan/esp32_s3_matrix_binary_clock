#include <cstdlib>
#include "ws_clock.h"
static uint16_t clock_color = Color(0, 2, 0);//灯泡亮度和颜色控制
Adafruit_NeoMatrix Matrix = Adafruit_NeoMatrix(8, 8, RGB_Control_PIN,
                                               NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_PROGRESSIVE,
                                               NEO_RGB + NEO_KHZ800);
int MatrixWidth = 0;
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
void rotateSquareMatrix() {  //向右旋转一次
  // uint8_t color[3] = { 0 };
  int layer, first, last, i, j;
  uint16_t temp;

  for (layer = 0; layer < 8 / 2; layer++) {
    first = layer;
    last = 8 - 1 - layer;

    for (i = first; i < last; i++) {
      // 保存左上角的值
      temp = Show_Data[first][i];
      Show_Data[first][i] = Show_Data[last - (i - first)][first];
      Show_Data[last - (i - first)][first] = Show_Data[last][last - (i - first)];
      Show_Data[last][last - (i - first)] = Show_Data[i][last];
      Show_Data[i][last] = temp;
    }
  }
}
void RGB_Leds() {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {

      Matrix.drawPixel(col, row, Show_Data[row][col]);
    }
  }
  Matrix.show();
}
void set_wifistatus_ok() {  //wifi ok 显示绿色
  uint16_t color = Color(0, 10, 0);
  Real_Data[0][0] = color;
}
void set_wifistatus_err() {  //wifi err 显示红色
  uint16_t color = Color(0, 10, 0);
  Real_Data[0][0] = color;
}
void set_ntp_ok() {  //wifi ok 显示绿色
  uint16_t color = Color(0, 10, 0);
  Real_Data[0][1] = color;
}
void set_ntp_err() {  //wifi err 显示红色
  uint16_t color = Color(0, 10, 0);
  Real_Data[0][1] = color;
}

void matrix_copy() {
  for (uint8_t row = 0; row < 8; row++) {
    for (uint8_t col = 0; col < 8; col++) {
      Show_Data[row][col] = Real_Data[row][col];
    }
  }
}
//将时间赋予时钟系统
void set_time(String ttime) {
  clock_time = ttime;
}
void set_Clock() {  //12:34:56
  uint8_t hour, min, sec;
  printf("timestring: %s\n", clock_time);
  hour = std::atoi(clock_time.substring(0, 2).c_str());
  min = std::atoi(clock_time.substring(3, 5).c_str());
  sec = std::atoi(clock_time.substring(6, 8).c_str());
  printf("h:%d,min:%d,sec:%d\n", hour, min, sec);
  set_hour(hour);
  set_min(min);
  set_sec(sec);

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
void set_hour(uint8_t hour) {
  binary_pixel(hour, Hour);
  for (int col = 1; col >= 0; col--) {
    for (int row = 4; row >= 0; row--) {
      if (Hour[row][col] == false) {  //这个点位不需要亮
        Real_Data[row + 3][col] = 0;
        printf("row:%d , col: %d\n", row, col);
      } else {
        Real_Data[row + 3][col] = clock_color;
      }
    }
  }
}
void set_min(uint8_t min) {
  binary_pixel(min, Min);
  for (int col = 1; col >= 0; col--) {
    for (int row = 4; row >= 0; row--) {
      if (Min[row][col] == false) {  //这个点位不需要亮
        Real_Data[row + 3][col + 3] = 0;
      } else {
        Real_Data[row + 3][col + 3] = clock_color;
      }
    }
  }
}
void set_sec(uint8_t sec) {
  binary_pixel(sec, Sec);
  for (int col = 1; col >= 0; col--) {
    for (int row = 4; row >= 0; row--) {
      if (Sec[row][col] == false) {  //这个点位不需要亮
        Real_Data[row + 3][col + 6] = 0;
      } else {
        Real_Data[row + 3][col + 6] = clock_color;
      }
    }
  }
}

// 16
// 8
// 4
// 2
// 1
void binary_pixel(uint8_t num, bool time[][2]) {  
  uint8_t last_index = 5;
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

  for (; last_index > 0; last_index--) {
    // printf("binary_pixel:for_num- %d\n", num);
    if (num % 2 == 1) {
      time[last_index - 1][1] = true;

    } else {
      time[last_index - 1][1] = false;
    }
    num = num / 2;

    // printf("-------------\n");
  }
}