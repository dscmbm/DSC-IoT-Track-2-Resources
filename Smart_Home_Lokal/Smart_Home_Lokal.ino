#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

#define lamp1  D4  //relay1 
#define lamp2  D5  //relay2
#define magnetpin  D7  //Magnetic switch Sensor
#define dhtpin     D8  //Temperature and Humidity Sensor

#define DHTTYPE DHT22   // DHT22  (AM2302)
DHT dht(dhtpin, DHTTYPE);
float h, t; 
String data, data2;

ESP8266WebServer server(80);

char ssid[] = "ESP_Wifi";
char password[] = "11223344";
String webPage = "";

IPAddress IP(192, 168, 3, 10);
IPAddress mask = (255, 255, 255, 0);

void setup(void) {
  webPage += "<h1>Smart Home Lokal Only</h1><p>Lamp 1 <a href=\"flame\"><button>ON</button></a>&nbsp;<a href=\"die\"><button>OFF</button></a></p><p>Lamp 2 <a href=\"l2flame\"><button>ON</button></a>&nbsp;<a href=\"l2die\"><button>OFF</button></a></p>";//<p>Temperature : t &nbsp;Humidity : h</p>";

  pinMode(lamp1, OUTPUT);
  pinMode(lamp2, OUTPUT);
  pinMode(magnetpin, INPUT_PULLUP);      // For Magnetic Reed Switch
  pinMode(dhtpin, INPUT_PULLUP);       // For DHT22
  digitalWrite(lamp1, LOW);
  digitalWrite(lamp2, LOW);
  Serial.begin(9600);
  dht.begin();
  delay(10);

  Serial.begin(9600);
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  WiFi.softAPConfig(IP, IP, mask);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", []() {
    server.send(200, "text/html", webPage + data + data2);
  });

  //turn on lamp 1
  server.on("/flame", []() {
    server.send(200, "text/html", webPage + data + data2);
    digitalWrite(lamp1, HIGH);
    delay(50);
  });

  //turn off lamp 1
  server.on("/die", []() {
    server.send(200, "text/html", webPage + data + data2);
    digitalWrite(lamp1, LOW);
    delay(50);
  });

  //turn on lamp 2
  server.on("/l2flame", []() {
    server.send(200, "text/html", webPage + data + data2);
    digitalWrite(lamp2, HIGH);
    delay(50);
  });

  //turn off lamp 2
  server.on("/l2die", []() {
    server.send(200, "text/html", webPage + data + data2);
    digitalWrite(lamp2, LOW);
    delay(300);
  });
  
  server.begin();
  Serial.println("HTTP server started");

}

void loop(void) {
  // Humidity and Temperature
  h = dht.readHumidity();
  t = dht.readTemperature();
  data = "";
  data += "<p>";
  data += "Humidity : ";
  data += h;
  data += "%  Temperature : ";
  data += t;
  data += "C";
  data += "</p>";
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%");
  Serial.print(" ");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println("°C");

  //Sensor Magnet
  boolean btnState = digitalRead(13);
  if (btnState != LOW) {
    data2 = "";
    data2 += "<p>";
    data2 += "Sensor Status : ";
    data2 += "The DOOR IS OPEN!";
    data2 += "</p>";
  } else {
    data2 = "";
    data2 += "<p>";
    data2 += "Sensor Status : ";
    data2 += "The House is Safe!";
    data2 += "</p>";
  }
  
  server.handleClient();
}
