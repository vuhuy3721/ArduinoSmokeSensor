#include <Wire.h>
#include <Adafruit_AHTX0.h>

// Cấu hình chân cho cảm biến khói (MP2)
#define SMOKE_SENSOR_PIN A6 // Đầu vào ADC

// Khởi tạo đối tượng cảm biến AHT20
Adafruit_AHTX0 aht;

// Hàm khởi tạo cảm biến AHT20
void initAHT20() {
    if (!aht.begin()) {
        Serial.println("Không tìm thấy AHT20");
        while (1) delay(10);
    }
    Serial.println("AHT20 đã sẵn sàng!");
}

// Hàm đọc dữ liệu từ cảm biến khói (MP2)
float readSmokeSensor() {
    int rawValue = analogRead(SMOKE_SENSOR_PIN); // Đọc giá trị ADC
    float voltage = (rawValue / 4095.0) * 3.3; // Chuyển đổi sang điện áp (3.3V tham chiếu)
    return voltage;
}

// Hàm đọc dữ liệu từ cảm biến AHT20
void readAHT20(float &temperature, float &humidity) {
    sensors_event_t humidityEvent, tempEvent;
    aht.getEvent(&humidityEvent, &tempEvent); // Lấy dữ liệu cảm biến

    temperature = tempEvent.temperature;
    humidity = humidityEvent.relative_humidity;
}

void setup() {
    Serial.begin(115200);
    while (!Serial);

    // Khởi tạo cảm biến
    initAHT20();
    pinMode(SMOKE_SENSOR_PIN, INPUT); // Cấu hình chân cảm biến khói là đầu vào

    Serial.println("Hệ thống đo nhiệt độ, độ ẩm và phát hiện khói đã sẵn sàng!");
}

void loop() {
    // Đọc dữ liệu từ AHT20
    float temperature, humidity;
    readAHT20(temperature, humidity);

    // Đọc dữ liệu từ cảm biến khói
    float smokeVoltage = readSmokeSensor();

    // Hiển thị kết quả
    Serial.print("Nhiệt độ: ");
    Serial.print(temperature);
    Serial.print(" °C, Độ ẩm: ");
    Serial.print(humidity);
    Serial.print(" %, Điện áp khói: ");
    Serial.print(smokeVoltage);
    Serial.println(" V");

    // Kiểm tra điều kiện phát hiện khói
    if (smokeVoltage > 2.5) { 
        Serial.println("CẢNH BÁO: Phát hiện khói!");
    } else {
        Serial.println("Không khí bình thường.");
    }

    delay(1000); 
}
