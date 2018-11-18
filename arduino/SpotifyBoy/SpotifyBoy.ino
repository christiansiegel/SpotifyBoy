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

// Pins are currently set for Arduino Mega 2560
const int SC_INTERRUPT = 1;  // interrupt 1 = pin 3
const int SI_PIN = 4;

constexpr unsigned long RECEIVE_BIT_DURATION_MICROS = 1e6 / 8192;  // us/Hz
volatile unsigned long last_receive = 0;
volatile int received_bits_count = 0;
volatile int received_byte = 0;

void setup() {
  Serial.begin(115200);

  pinMode(SI_PIN, INPUT_PULLUP);
  attachInterrupt(SC_INTERRUPT, receive_interrupt, RISING);
}

void receive_interrupt() {
  if (micros() - last_receive > RECEIVE_BIT_DURATION_MICROS) {
    // this took too long - must be a new byte sequence
    received_byte = 0;
    received_bits_count = 0;
  }

  int received_bit = digitalRead(SI_PIN);
  received_bits_count++;
  last_receive = micros();

  if (received_bit == HIGH) {
    bitSet(received_byte, 8 - received_bits_count);  // MSB first
  }

  if (received_bits_count == 8) {
    Serial.println(received_byte, DEC);
    received_byte = 0;
    received_bits_count = 0;
  }
}

void loop() {}
