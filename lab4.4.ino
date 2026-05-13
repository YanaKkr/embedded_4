#include <SPIFFS.h>
#include <Adafruit_BMP280.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define LDR_PIN 34
#define ONE_WIRE_BUS 4

Adafruit_BMP280 bmp;
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18(&oneWire);

void setup() {
    Serial.begin(115200);
    if(!SPIFFS.begin(true)) return;
    
    bmp.begin(0x76);
    ds18.begin();
    
    // Timer-based collection (Simulated with a loop)
    File file = SPIFFS.open("/sensors.csv", FILE_WRITE);
    if(!file) return;

    for(int i=0; i<10; i++) {
        ds18.requestTemperatures();
        float t1 = ds18.getTempCByIndex(0);
        float t2 = bmp.readTemperature();
        float p = bmp.readPressure();
        int ldr = analogRead(LDR_PIN);

        // Format: Time, DS18_Temp, BMP_Temp, Pressure, Light
        file.printf("%lu,%.2f,%.2f,%.2f,%d\n", millis(), t1, t2, p, ldr);
        Serial.println("Data row saved.");
        delay(2000);
    }
    file.close();
}

void loop() {}
