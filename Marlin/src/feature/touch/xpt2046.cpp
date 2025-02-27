/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(TOUCH_BUTTONS)

#include "xpt2046.h"
#include "../../inc/MarlinConfig.h"

#ifndef TOUCH_INT_PIN
  #define TOUCH_INT_PIN  -1
#endif
#ifndef TOUCH_MISO_PIN
  #define TOUCH_MISO_PIN MISO_PIN
#endif
#ifndef TOUCH_MOSI_PIN
  #define TOUCH_MOSI_PIN MOSI_PIN
#endif
#ifndef TOUCH_SCK_PIN
  #define TOUCH_SCK_PIN  SCK_PIN
#endif
#ifndef TOUCH_CS_PIN
  #define TOUCH_CS_PIN   CS_PIN
#endif

XPT2046 touch;
extern int8_t encoderDiff;

void XPT2046::init(void) {
  SET_INPUT(TOUCH_INT_PIN); // Pendrive interrupt pin, used as polling in getInTouch
  SET_INPUT(TOUCH_MISO_PIN);
  SET_OUTPUT(TOUCH_MOSI_PIN);

  OUT_WRITE(TOUCH_SCK_PIN, 0);
  OUT_WRITE(TOUCH_CS_PIN, 1);

  // Read once to enable pendrive status pin
  getInTouch(XPT2046_X);
}

#include "../../lcd/ultralcd.h" // For EN_C bit mask

uint8_t XPT2046::read_buttons() {
  int16_t tsoffsets[4] = { 0 };

  static uint32_t timeout = 0;
  if (PENDING(millis(), timeout)) return 0;
  timeout = millis() + 250;

  if (tsoffsets[0] + tsoffsets[1] == 0) {
    // Not yet set, so use defines as fallback...
    tsoffsets[0] = XPT2046_X_CALIBRATION;
    tsoffsets[1] = XPT2046_X_OFFSET;
    tsoffsets[2] = XPT2046_Y_CALIBRATION;
    tsoffsets[3] = XPT2046_Y_OFFSET;
  }

  // We rely on XPT2046 compatible mode to ADS7843, hence no Z1 and Z2 measurements possible.

  if (READ(TOUCH_INT_PIN)) return 0; // If HIGH there are no screen presses.
  const uint16_t x = uint16_t(((uint32_t(getInTouch(XPT2046_X))) * tsoffsets[0]) >> 16) + tsoffsets[1],
                 y = uint16_t(((uint32_t(getInTouch(XPT2046_Y))) * tsoffsets[2]) >> 16) + tsoffsets[3];
  if (READ(TOUCH_INT_PIN)) return 0; // Fingers must still be on the TS for a valid read.

  if (y < 185 || y > 224) return 0;

       if (WITHIN(x,  21,  98)) encoderDiff = -(ENCODER_STEPS_PER_MENU_ITEM) * ENCODER_PULSES_PER_STEP;
  else if (WITHIN(x, 121, 198)) encoderDiff =   ENCODER_STEPS_PER_MENU_ITEM  * ENCODER_PULSES_PER_STEP;
  else if (WITHIN(x, 221, 298)) return EN_C;

  return 0;
}

uint16_t XPT2046::getInTouch(const XPTCoordinate coordinate) {
  uint16_t data[3];

  OUT_WRITE(TOUCH_CS_PIN, LOW);

  const uint8_t coord = uint8_t(coordinate) | XPT2046_CONTROL | XPT2046_DFR_MODE;
  for (uint16_t i = 0; i < 3 ; i++) {
    for (uint8_t j = 0x80; j; j >>= 1) {
      WRITE(TOUCH_SCK_PIN, LOW);
      WRITE(TOUCH_MOSI_PIN, bool(coord & j));
      WRITE(TOUCH_SCK_PIN, HIGH);
    }

    data[i] = 0;
    for (uint16_t j = 0x8000; j; j >>= 1) {
      WRITE(TOUCH_SCK_PIN, LOW);
      if (READ(TOUCH_MISO_PIN)) data[i] |= j;
      WRITE(TOUCH_SCK_PIN, HIGH);
    }
    WRITE(TOUCH_SCK_PIN, LOW);
    data[i] >>= 4;
  }

  WRITE(TOUCH_CS_PIN, HIGH);

  uint16_t delta01 = _MAX(data[0], data[1]) - _MIN(data[0], data[1]),
           delta02 = _MAX(data[0], data[2]) - _MIN(data[0], data[2]),
           delta12 = _MAX(data[1], data[2]) - _MIN(data[1], data[2]);

  if (delta01 <= delta02 && delta01 <= delta12)
    return (data[0] + data[1]) >> 1;

  if (delta02 <= delta12)
    return (data[0] + data[2]) >> 1;

  return (data[1] + data[2]) >> 1;
}

#endif // TOUCH_BUTTONS
