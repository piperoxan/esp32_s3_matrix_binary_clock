#include "WiFi.h"
#include "esp_sntp.h"
#include "time.h"
#include <NTPClient.h>
#include "Arduino.h"

// struct tm *getTime();
void SntpInit();
String ntptime();