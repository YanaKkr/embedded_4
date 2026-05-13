#define LDR_PIN 34
#define ONE_WIRE_BUS 4

Adafruit_BMP280 bmp; 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18(&oneWire);

void setup() {
    Serial.begin(115200);
    
    // Ініціалізація FS
    if(!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
        return;
    }
    
    if (!bmp.begin(0x76)) { 
        Serial.println("BMP280 error");
    }
    ds18.begin();
}

void logData() {
    File file = SPIFFS.open("/data_log.csv", FILE_APPEND);
    if(!file) {
        Serial.println("Failed to open file");
        return;
    }

    ds18.requestTemperatures();
    float tempDS = ds18.getTempCByIndex(0);
    float tempBMP = bmp.readTemperature();
    float press = bmp.readPressure();
    int ldrValue = analogRead(LDR_PIN);

    // CSV формат: час, temp1, temp2, тиск, світло
    String dataRow = String(millis()) + "," + String(tempDS) + "," + 
                     String(tempBMP) + "," + String(press) + "," + String(ldrValue) + "\n";

    file.print(dataRow);
    file.close();
    Serial.print("Saved: " + dataRow);
}

void readFile() {
    File file = SPIFFS.open("/data_log.csv", FILE_READ);
    if(!file) return;
    
    Serial.println("--- Reading File Content ---");
    while(file.available()) {
        Serial.write(file.read());
    }
    file.close();
}

unsigned long lastLog = 0;
void loop() {
    if (millis() - lastLog > 5000) { // Запис кожні 5 секунд
        logData();
        lastLog = millis();
    }
    
    if(Serial.available() && Serial.read() == 'r') readFile();
}
