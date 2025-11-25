//连WiFi用的
#include "ws_wifi.h"
// #include "ws_ori_test.h"
#include "ws_clock.h"
const char*ssid="your wifi ssid";
const char* sssk="your wifi  pwd";
void Wifi_On() {
  WiFi.begin(ssid, sssk);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);  
    set_wifistatus_err();
  }
  set_wifistatus_ok();
}
void Wifi_Off(){
  WiFi.disconnect(true);   // 断开并清除配置
WiFi.mode(WIFI_OFF);     // 关闭 Wi-Fi 模块
}