#include "ws_grav.h"
#include "ws_btn.h"
#include "ws_wifi.h"
#include "ws_sntp.h"
#include "ws_clock.h"


String t__time;
struct tm *t_time;
//一个时钟, 通过wifi校时
void setup() {

  QMI8658_Init();
  printf("prepare leds");
  Matrix_Init();
  printf("prepare wifi");
  Conn_wifi();
  printf("prepare ntp");
  SntpInit();
  printf("prepare sync time");
}

void loop() {

  QMI8658_Loop();
  delay(1000);
  set_time(ntptime());  
  set_Clock();
  rotal_Leds(rota());
  // fake_time();
  RGB_Leds();
}
