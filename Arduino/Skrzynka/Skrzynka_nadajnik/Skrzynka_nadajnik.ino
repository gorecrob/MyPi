#include <VirtualWire.h>


#define kontaktronPin 9    // 15 nóżka µC
const int led_pin = 11;
const int transmit_pin = 10;
const int receive_pin = 2;
const int transmit_en_pin = 3;

const char *wiadomosc;

void setup() {
  // Initialise the IO and ISR
  vw_set_tx_pin(transmit_pin);
  vw_set_rx_pin(receive_pin);
  vw_set_ptt_pin(transmit_en_pin);
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);       // Bits per sec

  pinMode(kontaktronPin, INPUT_PULLUP);
  pinMode(led_pin, OUTPUT);
}

void loop() {

  if(digitalRead(kontaktronPin)==HIGH) {wiadomosc = "listy";}
  else                                 {wiadomosc = "pusto";}
  digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
  vw_send((uint8_t *)wiadomosc, strlen(wiadomosc));
  vw_wait_tx();
    digitalWrite(led_pin, LOW);
  delay(1000);
}
