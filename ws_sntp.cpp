//这里是时间同步的方法
#include "ws_sntp.h"
// #include "ws_ori_test.h"
#include "ws_clock.h"
#define TIME_ZONE 8
const char* ntpServer = "ntp2.aliyun.com";
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, ntpServer, TIME_ZONE * 3600, 1000 * 60 * 60);



String ntptime() {

  timeClient.update();
  set_ntp_ok();
  return timeClient.getFormattedTime();
}
void SntpInit() {
  sntp_init();

  esp_sntp_setservername(0, ntpServer);
  set_ntp_err();
  // printf("ntp connecting");
  timeClient.begin();
  // printf("ntp connected");
}