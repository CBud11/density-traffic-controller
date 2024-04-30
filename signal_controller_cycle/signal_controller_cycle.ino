#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char ssid[] = "";
char password[] = "";

class TrafficSignal {
public:
  TrafficSignal(int red, int yellow, int green, int echo, int trig)
  : m_Red(red), m_Yellow(yellow), m_Green(green), m_Echo(echo), m_Trig(trig) {
  }

  void attach() {
    pinMode(m_Red, OUTPUT);
    pinMode(m_Yellow, OUTPUT);
    pinMode(m_Green, OUTPUT);
    pinMode(m_Trig, OUTPUT);
    pinMode(m_Echo, INPUT);
    low();
    digitalWrite(m_Red, HIGH);
  }

  bool detected() {
    double dist = distance();
    Serial.print("Distance:");
    Serial.println(dist);
    return dist < 6.0;
  }

  void executeSequence(int vPinRed=-1, int vPinYellow=-1, int vPinGreen=-1) {
    low();
    digitalWrite(m_Red, HIGH);
    if (vPinRed!=-1) Blynk.virtualWrite(vPinRed, 255);
    delay(2000);
    digitalWrite(m_Red, LOW);
    if (vPinRed!=-1) Blynk.virtualWrite(vPinRed, 0);
    digitalWrite(m_Yellow, HIGH);
    if (vPinYellow!=-1) Blynk.virtualWrite(vPinYellow, 255);
    delay(2000);
    digitalWrite(m_Yellow, LOW);
    if (vPinYellow!=-1) Blynk.virtualWrite(vPinYellow, 0);
    digitalWrite(m_Green, HIGH);
    if (vPinGreen!=-1) Blynk.virtualWrite(vPinGreen, 255);
    delay(2000);
    digitalWrite(m_Green, LOW);
    if (vPinGreen!=-1) Blynk.virtualWrite(vPinGreen, 0);
    delay(200);
    digitalWrite(m_Red, HIGH);
    delay(200);
  }

private:
  int m_Red, m_Yellow, m_Green, m_Echo, m_Trig;

  double distance() {
    digitalWrite(m_Trig, LOW);
    delayMicroseconds(2);
    digitalWrite(m_Trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(m_Trig, LOW);
    long duration = pulseIn(m_Echo, HIGH);
    return 1.0*duration*0.034/2.0;
  }

  void low() {
    digitalWrite(m_Red, LOW);
    digitalWrite(m_Yellow, LOW);
    digitalWrite(m_Green, LOW);
  }
};

const int NSignals = 3;

//(int red, int yellow, int green, int echo, int trig)
TrafficSignal signals[NSignals] = {
  TrafficSignal(26,25,33,13,12),
  TrafficSignal(19,18,5,14,27),
  TrafficSignal(32,3,21,23,22)
};

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
  for (int i = 0; i < NSignals; ++i) {
    signals[i].attach();
  }
  Serial.println("Initialized...");
}

void loop() {
  Blynk.run();
  if (signals[0].detected()) {
    signals[0].executeSequence(V0, V1, V2);
  } 
  if (signals[1].detected()) {
    signals[1].executeSequence();
  } 
  if (signals[2].detected()) {
    signals[2].executeSequence();
  }
  delay(200);
}
