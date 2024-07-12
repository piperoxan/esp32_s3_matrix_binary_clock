#include "ws_grav.h"
#include "ws_wifi.h"
#include "ws_sntp.h"
#include "ws_clock.h"


//一个时钟, 通过wifi校时
void setup() {

  QMI8658_Init();
  printf("prepare leds");
  Matrix_Init();
  printf("prepare wifi");
  Conn_wifi();
  printf("prepare ntp");
  SntpInit();
}

void loop() {

  QMI8658_Loop(); //接收三轴传感器数据
  delay(1000);
  set_time(ntptime());  //获取ntp时间
  set_Clock(); //设置时间
  rotal_Leds(rota());//判定是否需要旋转点阵
  // fake_time();
  RGB_Leds();//亮
}
