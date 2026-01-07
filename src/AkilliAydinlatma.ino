#define ldrPin A0
#define ledPin 11
#define trigPin 8
#define echoPin 9
#define buzzerPin 12

int ldrValue;
int ldrThreshold = 50;  // Işık Kontrolü
long duration;
int distance;
int hareketThreshold = 30; // Mesafe Kontrolü 

bool onceHareketVardi = false;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}

void playDoorBell() {
  for (int i = 0; i < 2; i++) {
    // DING
    digitalWrite(buzzerPin, HIGH);
    delay(500);
    digitalWrite(buzzerPin, LOW);
    delay(300);
    // DONG
    digitalWrite(buzzerPin, HIGH);
    delay(1000);
    digitalWrite(buzzerPin, LOW);
    if (i < 1) delay(600);
  }
}

void loop() {
  ldrValue = analogRead(ldrPin);

  // HC-SR04 Mesafe
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH, 25000);
  distance = duration * 0.034 / 2;

  Serial.print("LDR: ");
  Serial.print(ldrValue);
  Serial.print("  Mesafe: ");
  Serial.print(distance);
  Serial.println(" cm");

  bool geceMi = (ldrValue < ldrThreshold);
  bool hareketVarMi = (distance > 0 && distance < hareketThreshold);

  // -------- LED kontrolü --------
  // Gece + hareket boyunca LED hep açık
  if (geceMi && hareketVarMi) {
    digitalWrite(ledPin, HIGH); 
  } else {
    digitalWrite(ledPin, LOW);
  }

  // -------- GÜNDÜZ --------
  if (!geceMi && hareketVarMi && !onceHareketVardi) {
    playDoorBell();
  }
  if (!geceMi && !hareketVarMi) {
    digitalWrite(buzzerPin, LOW);
  }

  // -------- GECE --------
  if (geceMi) {
    if (hareketVarMi) {
      digitalWrite(buzzerPin, HIGH);
      delay(150);
      digitalWrite(buzzerPin, LOW);
      delay(100);
    
    } else {
      digitalWrite(buzzerPin, LOW);
    }
  }

  onceHareketVardi = hareketVarMi;

  delay(10); 
}
