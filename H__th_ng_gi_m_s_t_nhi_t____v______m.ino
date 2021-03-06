#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPLROcjF94W"
#define BLYNK_DEVICE_NAME "Hiển thị độ ẩm"
#define BLYNK_AUTH_TOKEN "me0trhcx5ij4Ucdf9v0TYfqCwwR1eOt1"
//#define BLYNK_TEMPLATE_ID "TMPLoTsAFFH4"
//#define BLYNK_DEVICE_NAME "Hiển thị độ ẩm"
//#define BLYNK_AUTH_TOKEN "gnw1SxtNJRPJI7idv1KEZT2gz0fyf6xQ"
//Xử lí tính năng tự động kết nối app Blynk khi thay đổi wifi
//#define BLYNK_TEMPLATE_ID "TMPLoTsAFFH4"
//#define BLYNK_DEVICE_NAME "Mr Tai"

//#define BLYNK_FIRMWARE_VERSION        "0.1.0"

//#define APP_DEBUG
//#define USE_NODE_MCU_BOARD
//#include "BlynkEdgent.h"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"//thư viện DHT           
#include <SimpleTimer.h>
//BlynkTimer timer;
//int timerID1;
#define DHTTYPE DHT11
WidgetLED led(V0);//Đèn hiển thị trạng thái kết nối VXl <=> Blynk App
WidgetLED appledr(V2);// Đèn hiện thị trạng thái bật tắt máy bơm
#define dht_dpin 2 //D4
#define relay D2 //Relay bật tắt máy bơm
DHT dht(dht_dpin, DHTTYPE); 
SimpleTimer timer;

char auth[] = "me0trhcx5ij4Ucdf9v0TYfqCwwR1eOt1";
char ssid[] = "Tabi1";
char pass[] = "011111L1";
float t;
float h;
int sensor_value;

void setup()
{
    Serial.begin(9600);
    Blynk.begin(auth, ssid, pass);
    pinMode(relay, OUTPUT);
    dht.begin();//Khởi động cảm biến DHT
    //BlynkEdgent.begin();//Khởi động ché độ kết nối app Blynk
    //timerID1 = timer.setInterval(1000L,sendUptime);// Timer có nghĩa là cứ sau 1 giây sẽ gửi dữ liệu của hàm trong SendUptime lên Blynk 1 lần
    timer.setInterval(1000, sendUptime);
    while (WiFi.status() != WL_CONNECTED) {// Hàm check trạng thái kết nối wifi của esp32
      delay(500);
      Serial.println("Connecting to WiFi..");
      }
      Serial.println("Connected to the WiFi network");
}

void sendUptime()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 
  Serial.println("Humidity and temperature\n\n");
  Serial.println("Current humidity = ");
  Serial.println(h);
  Serial.println("%  ");
  Serial.println("temperature = ");
  Serial.println(t); 
  Blynk.virtualWrite(V6, t);//Khai báo chân ảo trên Blynk
  Blynk.virtualWrite(V5, h);
  
}

void loop()
{
  Blynk.run();
  //BlynkEdgent.run();//Khởi động Blynk
  timer.run();
  sensor_value = analogRead(A0);//ĐỌc giá trị độ ẩm đất ở chân A0
  sensor_value = map(sensor_value, 0, 1023, 0, 100);//Chuyển đổi dữ liệu Analog (1024) sang hiển thị phần trăm (max 100% tương ứng với 1024)
  Blynk.virtualWrite(V1,sensor_value);//truyền dữ liệu của Cảm biến SoilMoisture lên Blynk App
  if(led.getValue()) {//hàm hiển thị đèn trạng thái kết nối của VXl <=> Blynk App
    led.off();
    delay(500);
  }
  else {
    led.on();
    delay(500);
  }
  if(sensor_value<80){//hàm xử lý bơm khi Cảm biến SoilMoisture dưới 80% sẽ bật bơm và ngược lại. Đặt relay kích mức cao.
    appledr.on();//Nếu relay bật thì đèn trên App Blynk sẽ bật màu đỏ
    digitalWrite(relay, HIGH);//Relay bật làm bơm hoạt động. Do đặt relay ở chế độ High (Kích mức cao)
    
  }
  else{
    appledr.off();
    digitalWrite(relay, LOW);
  }
}
