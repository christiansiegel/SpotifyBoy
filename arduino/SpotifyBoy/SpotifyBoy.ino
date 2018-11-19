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
constexpr unsigned int HALF_BYTE_DURATION_MICROS = BIT_DURATION_MICROS * 4;

volatile bool ready_to_send = false;
volatile unsigned long last_receive = 0;
volatile int received_bits_count = 0;
volatile int received_byte = 0;

volatile int joypad = 0;
int last_joypad = 0;

char dummy_send_data[] = "This is SpotifyBoy!\n";
int dummy_send_data_pos = 0;

void setup() {
  Serial.begin(115200);

  pinMode(SC_PIN, INPUT_PULLUP);
  pinMode(SI_PIN, INPUT_PULLUP);
  pinMode(SO_PIN, OUTPUT);
  attachInterrupt(SC_INTERRUPT, receive_interrupt, RISING);
}

void receive_interrupt() {
  if (micros() - last_receive > HALF_BYTE_DURATION_MICROS) {
    // This took too long - must be a new byte sequence!
    received_byte = 0;
    received_bits_count = 0;
  }

  // Receive next bit.
  int received_bit = digitalRead(SI_PIN);
  received_bits_count++;
  last_receive = micros();

  // Save bit to byte.
  if (received_bit == HIGH) {
    bitSet(received_byte, 8 - received_bits_count);  // MSB first
  }

  // If full byte was received, save it and set to send mode again.
  if (received_bits_count == 8) {
    joypad = received_byte;
    received_byte = 0;
    received_bits_count = 0;
    ready_to_send = true;
  }
}

void loop() {
  // Print received joypad state if changed.
  if (joypad != last_joypad) {
    last_joypad = joypad;
    Serial.println(joypad, DEC);
  }

  if (ready_to_send) {
    // Disable SC interrupt.
    detachInterrupt(SC_INTERRUPT);
    pinMode(SC_PIN, OUTPUT);

    // Get next byte of our dummy text line.
    int byte_to_send = dummy_send_data[dummy_send_data_pos];
    if (++dummy_send_data_pos >= 20) dummy_send_data_pos = 0;

    // Wait for GameBoy to be receiving again.
    delay(1);

    // Send byte.
    for (int i = 0; i < 8; ++i) {
      int bit_to_send = (byte_to_send << i) & 0x80;  // MSB first
      digitalWrite(SO_PIN, bit_to_send);
      digitalWrite(SC_PIN, LOW);
      delayMicroseconds(HALF_BIT_DURATION_MICROS);
      digitalWrite(SC_PIN, HIGH);
      delayMicroseconds(HALF_BIT_DURATION_MICROS);
    }

    ready_to_send = false;

    // Enable SC interrupt again.
    pinMode(SC_PIN, INPUT_PULLUP);
    attachInterrupt(SC_INTERRUPT, receive_interrupt, RISING);
  }
}
