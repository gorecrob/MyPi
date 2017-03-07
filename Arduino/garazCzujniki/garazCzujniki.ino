// Program odczytuje temperaturę z czujnika

#include <OneWire.h>
#include <DS18B20.h>
#include <DHT.h>

// Numer pinu do którego podłaczasz czujnik DS18B20
#define ONEWIRE_PIN 2
// Numer pinu do którego podłaczasz czujnik DHT22
#define DHTPIN 3
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321


#define KONTAKTRON_OPEN 9    // 15 nóżka µC
#define KONTAKTRON_CLOSE 10  

const char *door_msg_old;
const char *door_msg_new;
const char *DHT22_msg;
const char *DS18B20_msg;

// Adres czujnika
byte address[8] = {0x28, 0x8B, 0xF8, 0x23, 0x6, 0x0, 0x0, 0xBD};

OneWire onewire(ONEWIRE_PIN);
DS18B20 sensors(&onewire);

DHT dht(DHTPIN, DHTTYPE);

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 10000;           // interval at which to blink (milliseconds)


void setup() {
  while(!Serial);
  Serial.begin(9600);
  Serial.println("Garaz !!!");
  sensors.begin();
  sensors.request(address);

  dht.begin();

  
  pinMode(KONTAKTRON_OPEN, INPUT_PULLUP);
  pinMode(KONTAKTRON_CLOSE, INPUT_PULLUP);  
}
void readDS18B20 () {
  if (sensors.available())
  {
    float temperature = sensors.readTemperature(address);
    Serial.print("Temperatua DS18B20: ");
    Serial.print(temperature);
    Serial.println(F(" 'C"));
    
    char temp_tmp [1];
    dtostrf(temperature, 3, 2, temp_tmp);
    
    //Serial.println (temp_tmp);
    sensors.request(address);
    
    String stringOne = "GR1|";
    stringOne += temp_tmp;
   int strLen = stringOne.length()+1;  
   char msg[1];
   stringOne.toCharArray(msg, strLen);
   //erial.println(stringOne);
   DS18B20_msg = msg;
   Serial.println (DS18B20_msg);
  }
}


void readDHT22() {
  // Wait a few seconds between measurements.

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
  
}

void readGarageDoor () {
  if(digitalRead(KONTAKTRON_OPEN)==HIGH &&digitalRead(KONTAKTRON_CLOSE)==HIGH)
  {
    door_msg_new = "RUCH";
  } 
  
  if(digitalRead(KONTAKTRON_OPEN)==LOW &&digitalRead(KONTAKTRON_CLOSE)==HIGH)
  {
    door_msg_new = "OTWARTE";
  } 
  if(digitalRead(KONTAKTRON_OPEN)==HIGH &&digitalRead(KONTAKTRON_CLOSE)==LOW)
  {
    door_msg_new = "ZAMKNIETE";
  } 
    
  Serial.println(door_msg_new);
  

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
  readGarageDoor ();
  
}



