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
#include <time.h>

void main(void) {
  time_t receive_start;

  while (1) {
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

    // Print the received byte to screen, or a 'T' if timed out.
    if (_io_status == IO_IDLE) {
      printf("%c", _io_in);
    } else {
      printf("T");
    }

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
