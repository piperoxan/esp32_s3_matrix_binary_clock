//连WiFi用的
#include "ws_wifi.h"
// #include "ws_ori_test.h"
#include "ws_clock.h"
const char*ssid="your wifi ssid";
const char* sssk="your wifi  pwd";
void Conn_wifi() {
  WiFi.begin(ssid, sssk);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);  
    set_wifistatus_err();
  }
  set_wifistatus_ok();
}