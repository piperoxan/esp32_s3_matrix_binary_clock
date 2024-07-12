#ifndef _WS_QMI8658_H_
#define _WS_QMI8658_H_
#include <Arduino.h>
#include <Wire.h>
#include "SensorQMI8658.hpp"
enum Direction {
  UP,
  RIGHT,
  DOWN,
  LEFT
};

Direction rota();
void QMI8658_Init();
void QMI8658_Loop();
#endif
