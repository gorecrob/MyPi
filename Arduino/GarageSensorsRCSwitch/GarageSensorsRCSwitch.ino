
#include <OneWire.h>
#include <DS18B20.h>
#include <DHT.h>
#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();
// Arduino connected pin number DS18B20
#define ONEWIRE_PIN 2       // 4 leg Atmega328 µC
// Arduino connected pin number DHT22
#define DHTPIN 3            // 5 leg Atmega328 µC
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321


#define KONTAKTRON_OPEN 9    // 15 leg Atmega328 µC
#define KONTAKTRON_CLOSE 10  // 16 leg Atmega328 µC

char *door_msg_old = "";
char *door_msg_new = "";
const char *DHT22_msg;
const char *DS18B20_msg;

// Adress DS18B20
const byte sensorAddress[8] = {0x28, 0x8B, 0xF8, 0x23, 0x6, 0x0, 0x0, 0xBD};

OneWire onewire(ONEWIRE_PIN);
DS18B20 sensors(&onewire);

DHT dht(DHTPIN, DHTTYPE);

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = (60000 * 5);           // interval at which to blink (milliseconds)
unsigned long previousMillis1 = 0;   

//433 sender
const int led_pin = 11;         // not used
const int transmit_pin = 12;    // 18 leg Atmega328 µC
const int receive_pin = 2;      // not used
const int transmit_en_pin = 3;  // not used


void setup() {
  while(!Serial);
  Serial.begin(9600);
  Serial.println("Garage Sensors !!!");
  
  sensors.begin();
  dht.begin();
  
  pinMode(KONTAKTRON_OPEN, INPUT_PULLUP);
  pinMode(KONTAKTRON_CLOSE, INPUT_PULLUP);  

  // Initialise the IO and ISR
  mySwitch.enableTransmit(transmit_pin);
  // Optional set pulse length.
  mySwitch.setPulseLength(186);


}



void readGarageDoor () {
  
  char * msg;

  if(digitalRead(KONTAKTRON_OPEN)==HIGH && digitalRead(KONTAKTRON_CLOSE)==HIGH)
  {
    //Serial.println("MOVE");
    msg = "MOVE"; //1011112
  } 
  
  if(digitalRead(KONTAKTRON_OPEN)==LOW && digitalRead(KONTAKTRON_CLOSE)==HIGH)
  {
    //Serial.println("OPEN");
    msg = "OPEN"; //1011110
  } 
  if(digitalRead(KONTAKTRON_OPEN)==HIGH && digitalRead(KONTAKTRON_CLOSE)==LOW)
  {
    //Serial.println("CLOSE");
    msg = "CLOSE"; //1011111
  } 
  
  if (door_msg_old != msg)
  {  
   for (int i=0; i<3;i++)
   {
     if (msg == "MOVE")
        mySwitch.send(1011112, 24);
     if (msg == "OPEN")
        mySwitch.send(1011110, 24);
     if (msg == "CLOSE")
        mySwitch.send(1011111, 24);
   }
    Serial.println(msg);
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
  int tempCx100x = (int)(temperature*100);

  long DS18B20_temp;
  char temp_tmp [1];
  dtostrf(tempCx100x, 2,0, temp_tmp);
    
  String stringOne = "11";
  
 if (tempCx100x < 0)
  {
    dtostrf(abs(tempCx100x), 2,0, temp_tmp);
    stringOne += "9";
    if (tempCx100x < 1000)
      stringOne += "0";
    stringOne += temp_tmp;
    Serial.println(stringOne);
  }
  else
   {
    dtostrf(abs(tempCx100x), 2,0, temp_tmp);
    stringOne += "0";
    if (tempCx100x < 1000)
      stringOne += "0";
    stringOne += temp_tmp;
    Serial.println(stringOne);
   }

   DS18B20_temp = stringOne.toInt();
   Serial.println (DS18B20_temp);
   for (int i=0; i<3;i++)
   {
      mySwitch.send(DS18B20_temp, 24);
   }

   
}


void readDHT22() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float humidity = dht.readHumidity();
  int humCx100 = (int)(humidity*100);
  // Read temperature as Celsius (the default)
  float temperature = dht.readTemperature();
  int tempCx100 = (int)(temperature*100);
  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  long DHT22_temp;
  char temp_tmp [1];
  dtostrf(tempCx100, 2,0, temp_tmp);
    
  String stringOne = "12";
  
 if (tempCx100 < 0)
  {
    dtostrf(abs(tempCx100), 2,0, temp_tmp);
    stringOne += "9";
    if (tempCx100 < 1000)
      stringOne += "0";
    stringOne += temp_tmp;
    Serial.println(stringOne);
  }
  else
   {
    dtostrf(abs(tempCx100), 2,0, temp_tmp);
    stringOne += "0";
    if (tempCx100 < 1000)
      stringOne += "0";
      
    stringOne += temp_tmp;
    Serial.println(stringOne);
   }

   DHT22_temp = stringOne.toInt();
   Serial.println (DHT22_temp);
   for (int i=0; i<3;i++)
   {
      mySwitch.send(DHT22_temp, 24);
   }
  long DHT22_hum = 1300000 + humCx100;
  Serial.println (DHT22_hum);
   for (int i=0; i<3; i++)
   {
      mySwitch.send(DHT22_hum, 24);
   }

  Serial.print("DHT22 --- Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C ");
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



