
#include <VirtualWire.h>
#include <OneWire.h>
#include <DS18B20.h>
#include <DHT.h>


// Arduino connected pin number DS18B20
#define ONEWIRE_PIN 2
// Arduino connected pin number DHT22
#define DHTPIN 3
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321


#define KONTAKTRON_OPEN 9    // 15 leg Atmega328 µC
#define KONTAKTRON_CLOSE 10  // 16 leg Atmega328 µC

char *door_msg_old = "";
char *door_msg_new = "";
const char *DHT22_msg;
const char *DS18B20_msg;
//int doorStatusNew = 0;
//int doorStatusOld = 0;
// Adress DS18B20
const byte sensorAddress[8] = {0x28, 0x8B, 0xF8, 0x23, 0x6, 0x0, 0x0, 0xBD};

OneWire onewire(ONEWIRE_PIN);
DS18B20 sensors(&onewire);

DHT dht(DHTPIN, DHTTYPE);

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 5000;           // interval at which to blink (milliseconds)
unsigned long previousMillis1 = 0;   

//433 sender
const int led_pin = 11;
const int transmit_pin = 12;
const int receive_pin = 2;
const int transmit_en_pin = 3;

int doorStatusOld;
void setup() {
  while(!Serial);
  Serial.begin(9600);
  Serial.println("Garage Sensors !!!");
  
  sensors.begin();
  dht.begin();
  
  pinMode(KONTAKTRON_OPEN, INPUT_PULLUP);
  pinMode(KONTAKTRON_CLOSE, INPUT_PULLUP);  

  // Initialise the IO and ISR
  vw_set_tx_pin(transmit_pin);
  vw_set_rx_pin(receive_pin);
  vw_set_ptt_pin(transmit_en_pin);
  vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);       // Bits per sec


}
void readGarageDoor () {
  char * msg;
  //int doorStatusOld = doorStatus;
  if(digitalRead(KONTAKTRON_OPEN)==HIGH && digitalRead(KONTAKTRON_CLOSE)==HIGH)
  {
    Serial.println("MOVE");
    msg = "MOVE";
   // vw_send((uint8_t *)msg, strlen(msg));
   // vw_wait_tx();
  } 
  
  if(digitalRead(KONTAKTRON_OPEN)==LOW && digitalRead(KONTAKTRON_CLOSE)==HIGH)
  {
    Serial.println("OPEN");
    msg = "OPEN";
   // vw_send((uint8_t *)msg, strlen(msg));
   // vw_wait_tx();
  } 
  if(digitalRead(KONTAKTRON_OPEN)==HIGH && digitalRead(KONTAKTRON_CLOSE)==LOW)
  {
    Serial.println("CLOSE");
    msg = "CLOSE";
   // vw_send((uint8_t *)msg, strlen(msg));
   // vw_wait_tx();
  } 
  if (door_msg_old != msg)
  {  
    vw_send((uint8_t *)msg, strlen(msg));
    vw_wait_tx();
    door_msg_old = msg; 
  }
}



void readDS18B20 () {

  // Requests sensor for measurement
  sensors.request(sensorAddress);
  
  // Waiting (block the program) for measurement reesults
  while(!sensors.available());
  
  // Reads the temperature from sensor
  float temperature = sensors.readTemperature(sensorAddress);
  
  // Prints the temperature on Serial Monitor
  Serial.print("DS18B20 --- Temperature: ");
  Serial.print(temperature);
  Serial.println(F(" 'C"));
    
  char temp_tmp [1];
  dtostrf(temperature, 3, 2, temp_tmp);
    
  String stringOne = "GR1|";
  stringOne += temp_tmp;
  int strLen = stringOne.length()+1;  
  char msg[1];
  stringOne.toCharArray(msg, strLen);
  
  DS18B20_msg = msg;
  Serial.println (DS18B20_msg);
//433 send temp
  vw_send((uint8_t *)DS18B20_msg, strlen(DS18B20_msg));
  vw_wait_tx();    

}


void readDHT22() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float temperature = dht.readTemperature();
 
  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  char temp_tmp [1];
  dtostrf(temperature, 3, 2, temp_tmp);
 
  String stringOne = "GR2|";
  stringOne += temp_tmp;
  stringOne +="|";
  
  char temp_hum [1];
  dtostrf(humidity, 3, 2, temp_hum);
  stringOne += temp_hum;

  Serial.print("DHT22 --- Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C ");

  int strLen = stringOne.length()+1;  
  char msg[1];
  stringOne.toCharArray(msg, strLen);

  DHT22_msg = msg;
  Serial.println (DHT22_msg);
//433 send temp
  vw_send((uint8_t *)DHT22_msg, strlen(DHT22_msg));
  vw_wait_tx();  
  
}

void loop() {

    unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval || currentMillis < previousMillis) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    readDS18B20 ();
    delay (1000);
    readDHT22();
  }
  delay (1000);
 readGarageDoor ();; 

}



