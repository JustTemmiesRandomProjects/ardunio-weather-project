//temp og fuktighet
#include <DHT.h>
#define DHTPIN 9
DHT dht22(DHTPIN, DHT22); 

//støv
#include <SoftwareSerial.h>
#include <SDS011.h>
#define PM_TX 2
#define PM_RX 3 
SDS011 sds;

//SD
#define SD_CS_PIN 10 
#include <SD.h>
String line;
File file;

int counter = 0;
float data[6];

void setup() {
  Serial.begin(9600);
  
  //støv 
  sds.begin(PM_TX, PM_RX); 
  
  //LEDs
  // pinMode(A1, OUTPUT); // Enable red LED control
  // pinMode(A0, OUTPUT); // Enable green LED control

  //SD
  pinMode(SD_CS_PIN, OUTPUT); // Activate CS-Pin control
  SD.begin(SD_CS_PIN); // Startup SD-card reader
  char filename[] = "beaver.txt"; // Define filename

  if (SD.exists(filename)) {
    file = SD.open(filename, O_WRITE | O_APPEND);
    file.println("--------------------");
    file.println("Filen ble åpnet på nytt.");
    file.println("--------------------");
  } else {
    file = SD.open(filename, O_CREAT | O_WRITE);
    file.println("Dette er den første linjen i filen.");
  }

  file.flush(); // Force saving data to SD-card
}

void printData() {
  for (int x = 0; x < 6; x++) {
    Serial.print(data[x]);
    Serial.print(';');
    file.print(data[x]);
    file.print(';');
  }
  Serial.println("");
  file.println("");
  file.flush();
}

float getTemp() {
  return dht22.readTemperature();
}

float getHumidity() {
  return dht22.readHumidity();
}

float getDust(int index1, int index2) {
  float pm25, pm10;
  int error;
  do {
  error = sds.read(&pm25, &pm10);
  } while (error != 0);

  data[index1] = pm10;
  data[index2] = pm25;
}

void loop() {

  printData();

  data[0] = counter; counter ++;
  data[1] = millis();
  data[2] = getTemp();
  data[3] = getHumidity();
  getDust(4, 5);

  delay(14507);
}