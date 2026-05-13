#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4 // Пін для DS18B20
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

float rawData[100];

// Медіанний фільтр (вікно 5)
float medianFilter(float* data, int size) {
    float window[5];
    // Спрощений приклад для однієї точки (зазвичай іде ковзним вікном)
    // Для завдання відфільтруємо весь масив, виводячи медіану
    std::sort(data, data + size);
    return data[size / 2];
}

// Проста реалізація Кальмана
struct KalmanFilter {
    float q = 0.1; // Процесний шум
    float r = 0.1; // Шум вимірювання
    float x = 20;  // Початкове значення
    float p = 1.0; // Помилка оцінки
    float k;       // Коефіцієнт Кальмана

    float update(float measurement) {
        p = p + q;
        k = p / (p + r);
        x = x + k * (measurement - x);
        p = (1 - k) * p;
        return x;
    }
};

void processSensors() {
    sensors.begin();
    KalmanFilter kf;
    
    Serial.println("Збір даних...");
    for(int i=0; i<100; i++) {
        sensors.requestTemperatures();
        rawData[i] = sensors.getTempCByIndex(0);
        float filteredK = kf.update(rawData[i]);
        Serial.printf("Raw: %.2f, Kalman: %.2f\n", rawData[i], filteredK);
        delay(1000);
    }
}