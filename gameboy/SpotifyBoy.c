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

#include <gb/gb.h>
#include <stdio.h>

void main(void) {
  UINT8 last_pad = 0x0;

  while (1) {
    UINT8 pad = joypad();

    if (pad != last_pad) {
      if (pad & J_UP) printf("U ");
      if (pad & J_DOWN) printf("D ");
      if (pad & J_LEFT) printf("L ");
      if (pad & J_RIGHT) printf("R ");
      if (pad & J_A) printf("A ");
      if (pad & J_B) printf("B ");
      printf("\n");

      _io_out = pad;
      send_byte();
      while (_io_status == IO_SENDING) {
        // wait
      }

      last_pad = pad;
    }
  }
}
