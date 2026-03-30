#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct {
  int16_t lx;
  int16_t ly;
  int16_t rx;
  int16_t ry;
  uint8_t l2;
  uint8_t r2;

  bool cross;
  bool circle;
  bool square;
  bool triangle;
  bool l1;
  bool r1;
  bool l3;
  bool r3;
  bool share;
  bool options;
  bool ps;
  bool touchpad;
  bool up;
  bool down;
  bool left;
  bool right;
  bool select;
  bool start;
  bool connected;
} ControllerState;
