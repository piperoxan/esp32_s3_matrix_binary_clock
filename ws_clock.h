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
#define Hour_OFFSET 0
#define MIN_OFFSET 3
#define SEC_OFFSET 6
#define CLOCK_HIGH 4
#define CLOCK_WIGHT 2


// struct LED{
//   uint8_t color[3];
// };
static uint16_t Show_Data[8][8], Real_Data[8][8], rotation_tmp[8][8];
static uint32_t Color(uint8_t r, uint8_t g, uint8_t b);
static bool Hour[5][2];
static bool Min[5][2];
static bool Sec[5][2];  //是否亮指定点位的灯
static void matrix_copy();
void RGB_Leds();
void Matrix_Init();
void fake_time();
void rotal_Leds(uint8_t dir);
void set_wifistatus_ok();
void set_wifistatus_err();
void set_ntp_ok();
void set_ntp_err();
static void rotateSquareMatrix();
static void set_hour(uint8_t hour);
static void set_min(uint8_t min);
static void set_sec(uint8_t sec);
// void set_time(String time);
void set_Clock(uint8_t t_hour,uint8_t t_min,uint8_t t_sec);
static bool (* binary_pixel(uint8_t num))[2];
static void set_num_display(bool time[][CLOCK_WIGHT], uint8_t offset);
