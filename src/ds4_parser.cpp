#include "ds4_parser.h"

namespace {

struct __attribute__((packed)) Ds4ReportHead {
  uint8_t report_id;
  uint8_t lx;
  uint8_t ly;
  uint8_t rx;
  uint8_t ry;
  uint8_t buttons1;
  uint8_t buttons2;
  uint8_t buttons3;
  uint8_t l2;
  uint8_t r2;
};

}  // namespace

bool parse_ds4_report(const uint8_t* report, uint16_t len, ControllerState* out_state) {
  if (!report || !out_state || len < sizeof(Ds4ReportHead)) {
    return false;
  }

  const auto* r = reinterpret_cast<const Ds4ReportHead*>(report);

  out_state->lx = static_cast<int16_t>(r->lx) - 128;
  out_state->ly = static_cast<int16_t>(r->ly) - 128;
  out_state->rx = static_cast<int16_t>(r->rx) - 128;
  out_state->ry = static_cast<int16_t>(r->ry) - 128;
  out_state->l2 = r->l2;
  out_state->r2 = r->r2;

  out_state->square = (r->buttons1 & 0x10u) != 0;
  out_state->cross = (r->buttons1 & 0x20u) != 0;
  out_state->circle = (r->buttons1 & 0x40u) != 0;
  out_state->triangle = (r->buttons1 & 0x80u) != 0;

  const uint8_t dpad = r->buttons1 & 0x0Fu;
  out_state->up = (dpad == 0u || dpad == 1u || dpad == 7u);
  out_state->right = (dpad == 1u || dpad == 2u || dpad == 3u);
  out_state->down = (dpad == 3u || dpad == 4u || dpad == 5u);
  out_state->left = (dpad == 5u || dpad == 6u || dpad == 7u);

  out_state->l1 = (r->buttons2 & 0x01u) != 0;
  out_state->r1 = (r->buttons2 & 0x02u) != 0;
  out_state->share = (r->buttons2 & 0x10u) != 0;
  out_state->options = (r->buttons2 & 0x20u) != 0;
  out_state->l3 = (r->buttons2 & 0x40u) != 0;
  out_state->r3 = (r->buttons2 & 0x80u) != 0;
  out_state->select = out_state->share;
  out_state->start = out_state->options;
  out_state->ps = (r->buttons3 & 0x01u) != 0;
  out_state->touchpad = (r->buttons3 & 0x02u) != 0;
  out_state->connected = true;
  return true;
}
