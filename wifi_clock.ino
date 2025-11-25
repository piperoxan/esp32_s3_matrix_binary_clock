#include "ws_grav.h"
#include "ws_wifi.h"
#include "ws_sntp.h"
#include "ws_clock.h"

// #include "driver/timer.h"

#include <Ticker.h>
Ticker ticker;  // 软件定时器对象
volatile uint8_t t_hour = 0;
volatile uint8_t t_min = 0;
volatile uint8_t t_sec = 0;

void tick() {
  // 每秒触发一次
  t_sec++;
  if (t_sec >= 60) {
    t_sec = 0;
    t_min++;
  }
  if (t_min >= 60) {
    t_min = 0;
    t_hour++;
  }
  if (t_hour >= 24) { t_hour = 0; }

  set_Clock(t_hour, t_min, t_sec);
  rotal_Leds(rota());
  RGB_Leds();
}

//一个时钟, 通过wifi校时
void setup() {

  QMI8658_Init();
  printf("prepare leds");
  Matrix_Init();
  printf("prepare wifi");
  Wifi_On();
  printf("prepare ntp");
  SntpInit();

  // 同步一次 NTP 获取初始时间
  String t = ntptime(); //只获取一次时间, 减少功耗
  parse_time(t, t_hour, t_min, t_sec);
  Wifi_Off();//拿到时间,关闭wifi,减少功耗和散热
  ticker.attach(1.0, tick);  // 1 秒周期
}

unsigned long last_ntp_sync = 0;
const unsigned long ntp_interval = 60UL * 1000;  // 60秒同步一次

void loop() {

  QMI8658_Loop();  //接收三轴传感器数据

  // // set_time(ntptime());  //获取ntp时间
  //  unsigned long now = millis();
  //   if (now - last_ntp_sync >= ntp_interval) {
  //       set_time(ntptime());
  //       last_ntp_sync = now;
  //   }
  // set_Clock(); //设置时间
  // rotal_Leds(rota());//判定是否需要旋转点阵
  // // fake_time();
  // RGB_Leds();//亮
  // delay(1000);
}
void parse_time(const String &t, volatile uint8_t &hour, volatile uint8_t &minute, volatile uint8_t &second) {
  hour = t.substring(0, 2).toInt();
  minute = t.substring(3, 5).toInt();
  second = t.substring(6, 8).toInt();
}
