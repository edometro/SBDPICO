#pragma once

#include <stdint.h>

#include "controller_state.h"

// Builds SBDBT-compatible 8-byte packet.
void build_sbdbt_packet(const ControllerState* state, uint8_t out_packet[8]);
