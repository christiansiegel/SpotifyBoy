/*
 * MIT License
 *
 * Copyright (c) 2018 Christian <christian.siegel@gmx.net>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <gb/drawing.h>
#include <gb/gb.h>
#include <stdio.h>
#include <time.h>

#define SIMULATION
#define SIMULATION_STOPWATCH

#define CMD_RESET 255
#define CMD_LINE 254
#define CMD_BOX 253
#define CMD_CIRCLE 252
#define CMD_COLOR 251
#define CMD_POINT 250
#define CMD_GOTO 249
#define CMD_CHAR 248
#define CMD_3POINTS 247

#ifdef SIMULATION
UINT8 simulation_data[] = {
    255,  // CMD_RESET
    // Draw black triangle:
    251, 3, 0, 1,        // CMD_COLOR; forecolor=BLACK, backcolor=WHITE, mode=OR
    254, 2, 2, 21, 2,    // CMD_LINE x1=2,  y1=2, x2=21, y2=2
    254, 2, 2, 12, 21,   // CMD_LINE x1=2,  y1=2, x2=12, y2=21
    254, 21, 2, 12, 21,  // CMD_LINE x1=21, y1=2, x2=12, y2=21
    // Draw dark grey box with light grey filling:
    251, 2, 1, 0,  // CMD_COLOR; color=DKGREY, bgcolor=LTGREY, mode=SOLID
    253, 25, 2, 46, 21, 1,  // CMD_BOX; x1=25, y1=2, x2=46, y2=21, style=FILL
    // Draw black box with no filling:
    251, 3, 0, 0,           // CMD_COLOR; color=BLACK, bgcolor=WHITE, mode=SOLID
    253, 30, 7, 51, 26, 0,  // CMD_BOX; x1=30, y1=7, x2=51, y2=26, style=NO_FILL
    // Draw light grey filled circle:
    251, 1, 2, 0,       // CMD_COLOR; color=LTGREY, bgcolor=DKGREY, mode=SOLID
    252, 75, 10, 9, 1,  // CMD_CIRCLE; x=75, y=10, r=9, style=FILL
    // Draw dark grey circle with no filling:
    251, 2, 2, 0,       // CMD_COLOR; color=DKGREY, bgcolor=DKGREY, mode=SOLID
    252, 80, 15, 9, 0,  // CMD_CIRCLE; x=80, y=15, r=9, style=NO_FILL
    // Draw black point:
    251, 3, 0, 1,  // CMD_COLOR; forecolor=BLACK, backcolor=WHITE, mode=OR
    250, 80, 15,   // CMD_POINT; x=80, y=15
    // Draw ligh grey letters on black background:
    251, 1, 3, 2,  // CMD_COLOR; forecolor=BLACK, backcolor=WHITE, mode=OR
    249, 15, 1,    // CMD_GOTO; x=15cells, y=1cell
    248, 'A',      // CMD_CHAR; char=A
    248, 'B',      // CMD_CHAR; char=B
    248, 'C',      // CMD_CHAR; char=C
    // Draw image:
    247, 30, 65, 0, 0, 0, 0, 16, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    0, 0, 0, 0, 0, 247, 30, 50, 0, 0, 0, 0, 21, 21, 41, 26, 21, 21, 21, 21, 21,
    21, 42, 26, 1, 0, 0, 0, 0, 0, 247, 30, 71, 0, 0, 0, 0, 16, 21, 37, 43, 62,
    47, 21, 21, 21, 41, 42, 21, 21, 1, 0, 0, 0, 0, 247, 30, 37, 0, 0, 0, 16, 21,
    37, 42, 21, 21, 21, 21, 21, 21, 41, 42, 21, 0, 0, 0, 0, 0, 0, 247, 30, 40,
    0, 0, 0, 16, 21, 21, 42, 22, 21, 21, 21, 21, 21, 41, 42, 22, 0, 0, 0, 0, 0,
    0, 247, 30, 55, 0, 0, 0, 0, 20, 21, 41, 42, 21, 21, 21, 21, 21, 21, 41, 26,
    5, 0, 0, 0, 0, 0, 247, 30, 31, 0, 0, 0, 0, 20, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 5, 0, 0, 0, 0, 0, 0, 247, 30, 81, 0, 0, 0, 0, 0, 21, 21, 21, 21,
    21, 26, 21, 22, 22, 37, 25, 25, 6, 0, 0, 0, 0, 247, 30, 91, 0, 0, 0, 0, 0,
    20, 21, 21, 21, 21, 21, 21, 21, 1, 0, 0, 0, 0, 0, 0, 0, 0, 247, 30, 90, 0,
    0, 0, 0, 0, 20, 21, 21, 21, 21, 21, 21, 21, 21, 21, 1, 0, 0, 0, 0, 0, 0,
    247, 30, 86, 0, 0, 0, 0, 0, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 38, 21,
    1, 0, 0, 0, 0, 247, 30, 30, 0, 0, 0, 0, 16, 21, 16, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 247, 30, 74, 0, 0, 0, 0, 16, 21, 21, 37, 43, 22, 21,
    21, 21, 21, 37, 21, 21, 1, 0, 0, 0, 0, 247, 30, 58, 0, 0, 0, 0, 20, 21, 21,
    22, 21, 21, 21, 21, 21, 21, 21, 21, 21, 0, 0, 0, 0, 0, 247, 30, 49, 0, 0, 0,
    0, 21, 21, 41, 26, 21, 21, 21, 21, 21, 21, 42, 26, 1, 0, 0, 0, 0, 0, 247,
    30, 36, 0, 0, 0, 16, 21, 37, 42, 22, 37, 42, 21, 21, 42, 42, 42, 5, 0, 0, 0,
    0, 0, 0, 247, 30, 66, 0, 0, 0, 0, 16, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 41, 22, 1, 0, 0, 0, 0, 247, 30, 89, 0, 0, 0, 0, 0, 20, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 21, 1, 0, 0, 0, 0, 0, 247, 30, 70, 0, 0, 0, 0, 16, 21,
    21, 58, 62, 42, 21, 21, 21, 37, 42, 41, 22, 1, 0, 0, 0, 0, 247, 30, 73, 0,
    0, 0, 0, 16, 21, 21, 42, 42, 42, 21, 21, 21, 21, 26, 21, 21, 1, 0, 0, 0, 0,
    247, 30, 87, 0, 0, 0, 0, 0, 20, 21, 21, 21, 21, 21, 21, 21, 21, 21, 25, 21,
    0, 0, 0, 0, 0, 247, 30, 80, 0, 0, 0, 0, 0, 21, 21, 21, 21, 21, 42, 21, 42,
    21, 21, 25, 38, 5, 0, 0, 0, 0, 247, 30, 47, 0, 0, 0, 0, 21, 21, 42, 26, 21,
    21, 21, 21, 21, 37, 42, 26, 1, 0, 0, 0, 0, 0, 247, 30, 33, 0, 0, 0, 16, 21,
    21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 5, 0, 0, 0, 0, 0, 0, 247, 30, 85, 0,
    0, 0, 0, 0, 21, 21, 21, 21, 21, 21, 21, 21, 21, 37, 21, 21, 1, 0, 0, 0, 0,
    247, 30, 64, 0, 0, 0, 0, 16, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    0, 0, 0, 0, 0, 247, 30, 77, 0, 0, 0, 0, 0, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 5, 0, 0, 0, 0, 247, 30, 44, 0, 0, 0, 16, 21, 21, 42, 22, 21,
    21, 21, 21, 21, 37, 42, 26, 1, 0, 0, 0, 0, 0, 247, 30, 42, 0, 0, 0, 16, 21,
    21, 42, 22, 21, 21, 21, 21, 21, 41, 42, 22, 0, 0, 0, 0, 0, 0, 247, 30, 43,
    0, 0, 0, 16, 21, 21, 42, 22, 21, 21, 21, 21, 21, 37, 42, 22, 0, 0, 0, 0, 0,
    0, 247, 30, 68, 0, 0, 0, 0, 16, 21, 21, 53, 42, 21, 21, 21, 21, 21, 21, 42,
    26, 1, 0, 0, 0, 0, 247, 30, 88, 0, 0, 0, 0, 0, 20, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 5, 0, 0, 0, 0, 0, 247, 30, 56, 0, 0, 0, 0, 20, 21, 41, 42,
    21, 21, 21, 21, 21, 21, 42, 22, 5, 0, 0, 0, 0, 0, 247, 30, 59, 0, 0, 0, 0,
    20, 21, 21, 21, 21, 37, 38, 42, 42, 21, 21, 21, 21, 0, 0, 0, 0, 0, 247, 30,
    53, 0, 0, 0, 0, 21, 21, 41, 26, 21, 21, 21, 21, 21, 21, 42, 26, 5, 0, 0, 0,
    0, 0, 247, 30, 51, 0, 0, 0, 0, 21, 21, 41, 26, 21, 21, 21, 21, 21, 21, 42,
    26, 5, 0, 0, 0, 0, 0, 247, 30, 32, 0, 0, 0, 0, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 5, 0, 0, 0, 0, 0, 0, 247, 30, 52, 0, 0, 0, 0, 21, 21, 41,
    26, 21, 21, 21, 21, 21, 21, 42, 26, 5, 0, 0, 0, 0, 0, 247, 30, 39, 0, 0, 0,
    16, 21, 21, 42, 22, 21, 21, 21, 21, 21, 41, 42, 22, 0, 0, 0, 0, 0, 0, 247,
    30, 72, 0, 0, 0, 0, 16, 21, 21, 43, 63, 47, 21, 21, 21, 37, 42, 21, 25, 1,
    0, 0, 0, 0, 247, 30, 79, 0, 0, 0, 0, 0, 21, 21, 21, 21, 21, 37, 22, 41, 22,
    21, 37, 37, 5, 0, 0, 0, 0, 247, 30, 67, 0, 0, 0, 0, 16, 21, 21, 37, 42, 21,
    21, 21, 21, 21, 21, 41, 26, 1, 0, 0, 0, 0, 247, 30, 83, 0, 0, 0, 0, 0, 21,
    21, 21, 21, 21, 26, 21, 21, 21, 25, 22, 37, 5, 0, 0, 0, 0, 247, 30, 78, 0,
    0, 0, 0, 0, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 25, 5, 0, 0, 0, 0,
    247, 30, 45, 0, 0, 0, 16, 21, 21, 42, 22, 21, 21, 21, 21, 21, 37, 42, 26, 1,
    0, 0, 0, 0, 0, 247, 30, 62, 0, 0, 0, 0, 20, 21, 21, 21, 21, 21, 21, 21, 21,
    21, 21, 21, 21, 0, 0, 0, 0, 0, 247, 30, 60, 0, 0, 0, 0, 20, 21, 37, 42, 38,
    42, 42, 26, 38, 21, 21, 21, 21, 0, 0, 0, 0, 0, 247, 30, 82, 0, 0, 0, 0, 0,
    21, 21, 21, 21, 21, 21, 21, 25, 21, 21, 38, 25, 5, 0, 0, 0, 0, 247, 30, 46,
    0, 0, 0, 0, 21, 21, 42, 26, 21, 21, 21, 21, 21, 37, 42, 26, 1, 0, 0, 0, 0,
    0, 247, 30, 54, 0, 0, 0, 0, 21, 21, 41, 42, 21, 21, 21, 21, 21, 21, 42, 26,
    5, 0, 0, 0, 0, 0, 247, 30, 61, 0, 0, 0, 0, 20, 21, 37, 38, 21, 21, 21, 21,
    21, 21, 21, 21, 21, 0, 0, 0, 0, 0, 247, 30, 57, 0, 0, 0, 0, 20, 21, 41, 42,
    42, 42, 42, 42, 41, 42, 26, 21, 5, 0, 0, 0, 0, 0, 247, 30, 93, 0, 0, 0, 0,
    0, 16, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 247, 30, 63, 0, 0, 0,
    0, 20, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 0, 0, 0, 0, 0, 247,
    30, 41, 0, 0, 0, 16, 21, 21, 42, 22, 21, 21, 21, 21, 21, 41, 42, 22, 0, 0,
    0, 0, 0, 0, 247, 30, 84, 0, 0, 0, 0, 0, 21, 21, 21, 21, 21, 21, 21, 21, 21,
    37, 25, 38, 5, 0, 0, 0, 0, 247, 30, 92, 0, 0, 0, 0, 0, 20, 21, 21, 21, 21,
    21, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 247, 30, 35, 0, 0, 0, 16, 21, 37, 42,
    42, 42, 42, 42, 42, 42, 42, 42, 5, 0, 0, 0, 0, 0, 0, 247, 30, 38, 0, 0, 0,
    16, 21, 21, 42, 22, 21, 21, 21, 21, 21, 41, 42, 22, 0, 0, 0, 0, 0, 0, 247,
    30, 34, 0, 0, 0, 16, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 5, 0, 0, 0,
    0, 0, 0, 247, 30, 48, 0, 0, 0, 0, 21, 21, 42, 26, 21, 21, 21, 21, 21, 37,
    42, 26, 1, 0, 0, 0, 0, 0, 247, 30, 76, 0, 0, 0, 0, 0, 21, 21, 21, 42, 22,
    21, 21, 21, 21, 21, 21, 21, 5, 0, 0, 0, 0, 247, 30, 75, 0, 0, 0, 0, 0, 21,
    21, 21, 43, 22, 21, 21, 21, 21, 21, 21, 21, 5, 0, 0, 0, 0, 247, 30, 69, 0,
    0, 0, 0, 16, 21, 21, 53, 42, 21, 21, 21, 21, 37, 42, 42, 26, 1, 0, 0, 0, 0};
size_t simulation_data_pos = 0;
#endif

UINT8 current_command = 255;
UINT8 current_command_params[5];
UINT8 current_command_params_pos = 0;
UINT8 current_x = 0;
UINT8 current_y = 0;
UINT8 current_x_cell = 0;
UINT8 current_y_cell = 0;

void handle(UINT8 byte) {
  // Detect new command and reset parameters
  if (byte >= 220) {
    current_command = byte;
    current_command_params_pos = 0;
    return;
  }

  // Cache parameters
  if (current_command_params_pos < sizeof(current_command_params)) {
    current_command_params[current_command_params_pos++] = byte;
  }

  // Execute commands if all necessary parameters received
  if (current_command == CMD_3POINTS) {
    if (current_command_params_pos == 2) {
      current_x = current_command_params[0];
      current_y = current_command_params[1];
    } else if (current_command_params_pos > 2) {
      plot(current_x++, current_y, byte & 0x3, OR);
      plot(current_x++, current_y, (byte >> 2) & 0x3, OR);
      plot(current_x++, current_y, (byte >> 4) & 0x3, OR);
    }
  } else if (current_command == CMD_GOTO && current_command_params_pos == 2) {
    current_x_cell = current_command_params[0];
    current_y_cell = current_command_params[1];
    gotogxy(current_x_cell, current_y_cell);
  } else if (current_command == CMD_LINE && current_command_params_pos == 4) {
    UINT8 x1 = current_command_params[0];
    UINT8 y1 = current_command_params[1];
    UINT8 x2 = current_command_params[2];
    UINT8 y2 = current_command_params[3];
    line(x1, y1, x2, y2);
  } else if (current_command == CMD_BOX && current_command_params_pos == 5) {
    UINT8 x1 = current_command_params[0];
    UINT8 y1 = current_command_params[1];
    UINT8 x2 = current_command_params[2];
    UINT8 y2 = current_command_params[3];
    UINT8 style = current_command_params[4];
    box(x1, y1, x2, y2, style);
  } else if (current_command == CMD_CIRCLE && current_command_params_pos == 4) {
    UINT8 x = current_command_params[0];
    UINT8 y = current_command_params[1];
    UINT8 radius = current_command_params[2];
    UINT8 style = current_command_params[3];
    circle(x, y, radius, style);
  } else if (current_command == CMD_COLOR && current_command_params_pos == 3) {
    UINT8 forecolor = current_command_params[0];
    UINT8 backcolor = current_command_params[1];
    UINT8 mode = current_command_params[2];
    color(forecolor, backcolor, mode);
  } else if (current_command == CMD_POINT && current_command_params_pos == 2) {
    UINT8 x = current_command_params[0];
    UINT8 y = current_command_params[1];
    plot_point(x, y);
  } else if (current_command == CMD_CHAR && current_command_params_pos == 1) {
    UINT8 c = current_command_params[0];
    wrtchr(c);
    gotogxy(++current_x_cell, current_y_cell);
  }
}

void main(void) {
  time_t receive_start;

  while (1) {
#ifdef SIMULATION
// Feed bytes from simulation data instead of actual receiving
#ifdef SIMULATION_STOPWATCH
    if (simulation_data_pos == sizeof(simulation_data) - 1) {
      color(3, 0, 1);
      gotogxy(0, 0);
      wrtchr('0' + (clock() / 10));
      gotogxy(1, 0);
      wrtchr('0' + (clock() % 10));
    }
#endif
    if (simulation_data_pos >= sizeof(simulation_data)) continue;
    handle(simulation_data[simulation_data_pos++]);
#else
    // Start receiving next byte.
    receive_byte();

    // Wait while the byte is received, or timeout after 10ms
    // (1/CLOCKS_PER_SEC).
    receive_start = clock();
    while (_io_status == IO_RECEIVING) {
      if (clock() - receive_start > 1) {
        break;  // timeout
      }
    }

    // Handle received byte
    if (_io_status == IO_IDLE) {
      handle(_io_in);
    }
#endif
    // Wait for Arduino to be receiving again.
    delay(1);

    // Send button status.
    _io_out = joypad();
    send_byte();
    while (_io_status == IO_SENDING) {
      // wait until finished
    }
  }
}
