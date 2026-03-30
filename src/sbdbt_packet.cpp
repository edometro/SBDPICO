#include "sbdbt_packet.h"

namespace {

uint8_t clamp7(int value) {
  if (value < -128) value = -128;
  if (value > 127) value = 127;
  return static_cast<uint8_t>((value + 128) >> 1);
}

}  // namespace

void build_sbdbt_packet(const ControllerState* state, uint8_t out_packet[8]) {
  uint8_t btn1 = 0;
  btn1 |= state->select ? 0x01u : 0;
  btn1 |= state->l3 ? 0x02u : 0;
  btn1 |= state->r3 ? 0x04u : 0;
  btn1 |= state->start ? 0x08u : 0;
  btn1 |= state->up ? 0x10u : 0;
  btn1 |= state->right ? 0x20u : 0;
  btn1 |= state->down ? 0x40u : 0;
  btn1 |= state->left ? 0x80u : 0;

  uint8_t btn2 = 0;
  btn2 |= (state->l2 > 100u) ? 0x01u : 0;
  btn2 |= (state->r2 > 100u) ? 0x02u : 0;
  btn2 |= state->l1 ? 0x04u : 0;
  btn2 |= state->r1 ? 0x08u : 0;
  btn2 |= state->triangle ? 0x10u : 0;
  btn2 |= state->circle ? 0x20u : 0;
  btn2 |= state->cross ? 0x40u : 0;
  btn2 |= state->square ? 0x80u : 0;

  out_packet[0] = 0x80;
  out_packet[1] = btn1;
  out_packet[2] = btn2;
  out_packet[3] = clamp7(state->lx);
  out_packet[4] = clamp7(state->ly);
  out_packet[5] = clamp7(state->rx);
  out_packet[6] = clamp7(state->ry);
  out_packet[7] =
      static_cast<uint8_t>((btn1 + btn2 + out_packet[3] + out_packet[4] + out_packet[5] +
                            out_packet[6]) &
                           0x7Fu);
}
