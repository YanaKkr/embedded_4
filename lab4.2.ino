#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float readings[100];

float getMedian(float* data, int n) {
    // Small sort for the window
    for (int i = 0; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            if (data[i] > data[j]) {
                float temp = data[i];
                data[i] = data[j];
                data[j] = temp;
            }
        }
    }
    return data[n/2];
}

void setup() {
    Serial.begin(115200);
    sensors.begin();
    
    Serial.println("Collecting 100 samples...");
    for(int i=0; i<100; i++) {
        sensors.requestTemperatures();
        readings[i] = sensors.getTempCByIndex(0);
        delay(1000); // 1-2 second interval
        Serial.print(".");
    }
    
    float medianValue = getMedian(readings, 100);
    Serial.printf("\nFiltered Median Temperature: %.2f C\n", medianValue);
}

void loop() {}
