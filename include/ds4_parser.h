#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "controller_state.h"

bool parse_ds4_report(const uint8_t* report, uint16_t len, ControllerState* out_state);
