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
  }

  bool detected() {
    double dist = distance();
    Serial.print("Distance:");
    Serial.println(dist);
    return dist < 6.0;
  }

  void update() {
    digitalWrite(m_Red, HIGH);
    delay(200);
  }

  void executeSequence() {
    low();
    digitalWrite(m_Red, HIGH);
    delay(2000);
    digitalWrite(m_Red, LOW);
    digitalWrite(m_Yellow, HIGH);
    delay(2000);
    digitalWrite(m_Yellow, LOW);
    digitalWrite(m_Green, HIGH);
    delay(2000);
    digitalWrite(m_Green, LOW);
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

TrafficSignal signals[NSignals] = {
  TrafficSignal(26,25,33,13,12),
  TrafficSignal(19,18,5,14,27),
  TrafficSignal(32,3,21,23,22)
};

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < NSignals; ++i) {
    signals[i].attach();
  }
}

void loop() {
  for (int i = 0; i < NSignals; ++i) {
    signals[i].update();
  }
  if (signals[0].detected()) {
    signals[0].executeSequence();
  } else if (signals[1].detected()) {
    signals[1].executeSequence();
  } else if (signals[2].detected()) {
    signals[2].executeSequence();
  }
  delay(200);
}
