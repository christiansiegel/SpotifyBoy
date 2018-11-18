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

const int SC_PIN = 3;
const int SI_PIN = 4;
const int SO_PIN = 5;

constexpr int SC_INTERRUPT = digitalPinToInterrupt(SC_PIN);

const unsigned int CLOCK = 8192;  // Hz

constexpr unsigned int BIT_DURATION_MICROS = 1e6 / CLOCK;
constexpr unsigned int HALF_BIT_DURATION_MICROS = BIT_DURATION_MICROS / 2;

volatile bool receiving = true;
volatile unsigned long last_receive = 0;
volatile int received_bits_count = 0;
volatile int received_byte = 0;

volatile int joypad = 0;
int last_joypad = 0;

void setup() {
  Serial.begin(115200);

  pinMode(SC_PIN, INPUT_PULLUP);
  pinMode(SI_PIN, INPUT_PULLUP);
  pinMode(SO_PIN, OUTPUT);
  attachInterrupt(SC_INTERRUPT, receive_interrupt, RISING);
}

void receive_interrupt() {
  receiving = true;

  if (micros() - last_receive > BIT_DURATION_MICROS) {
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
    joypad = received_byte;
    received_byte = 0;
    received_bits_count = 0;
    receiving = false;
  }
}

void loop() {
  if (joypad != last_joypad) {
    last_joypad = joypad;
    Serial.println(joypad, DEC);
  }

  if (Serial.available() > 0 && !receiving) {
    detachInterrupt(SC_INTERRUPT);
    pinMode(SC_PIN, OUTPUT);

    delay(5);  // not sure yet why I need this..

    int byte_to_send = Serial.read();

    for (int i = 0; i < 8; ++i) {
      int bit_to_send = (byte_to_send << i) & 0x80;  // MSB first
      digitalWrite(SO_PIN, bit_to_send);
      digitalWrite(SC_PIN, LOW);
      delayMicroseconds(HALF_BIT_DURATION_MICROS);
      digitalWrite(SC_PIN, HIGH);
      delayMicroseconds(HALF_BIT_DURATION_MICROS);
    }

    pinMode(SC_PIN, INPUT_PULLUP);
    attachInterrupt(SC_INTERRUPT, receive_interrupt, RISING);
  }
}
