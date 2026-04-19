#include <Servo.h>

#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

// --- تعريف دبابيس الـ RGB ---
#define redPin 2
#define greenPin 12
#define bluePin 13


int tiempo1 = 30;
int tiempo2 = 1000;

Servo Cintura;  // Servo for Waist control
Servo Hombro;   // Servo for Shoulder control
Servo Codo;     // Servo for Elbow control
Servo Pinzas;   // Servo for Gripper control

int ang;

void pickAndPlace(int dropAngle) {
  // 1. Move waist to pickup position
  Cintura.write(150);
  delay(500);

  // 2. Lower
  Hombro.write(130);
  Codo.write(20);
  delay(600);

  // 3. Extend codo
  Codo.write(0);
  delay(400);

  // 4. Grip
  Pinzas.write(90);
  delay(500);

  // 5. Contract
  Codo.write(20);
  delay(400);

  // 6. Lift
  Hombro.write(50);
  Codo.write(30);
  delay(600);

  // 7. Rotate to drop
  Cintura.write(dropAngle);
  delay(500);

  Hombro.write(130);
  Codo.write(20);
  delay(600);

  // 8. Let go
  Pinzas.write(0);
  delay(500);

  Hombro.write(50);
  Codo.write(30);
  delay(600);

  // 9. Home
  Cintura.write(130);
  Hombro.write(50);
  Codo.write(30);
  delay(500);
}
void redCase()   { pickAndPlace(40);  }  // placeholder drop angle
void greenCase() { pickAndPlace(60);  }  // placeholder drop angle
void blueCase()  { pickAndPlace(90); }  // placeholder drop angle

void setup() {

  Serial.begin(9600);
  
  // إعداد الحساس كمداخل ومخارج
  pinMode(S0, OUTPUT); pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT); pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  
  // إعداد الـ LED
  pinMode(redPin, OUTPUT); 
  pinMode(greenPin, OUTPUT); 
  pinMode(bluePin, OUTPUT);

  // ضبط تردد الحساس لـ 20% (الأفضل للأردوينو)
  digitalWrite(S0, HIGH); 
  digitalWrite(S1, LOW);

  Serial.println("Starting Color Detection...");

// ===============================================
// servo startig
// ===============================================


  Cintura.attach(3);  // Waist servo connected to pin 6
  Hombro.attach(11);   // Shoulder servo connected to pin 9
  Codo.attach(10);    // Elbow servo connected to pin 10
  Pinzas.attach(9);  // Gripper servo connected to pin 11

  Cintura.write(130);  // Initial position waist = 90 degrees ==== 1s3 65 ===
  Hombro.write(50);   // Initial position shoulder = 20 degrees
  Codo.write(30);     // Initial position elbow = 50 degrees
  Pinzas.write(0);    // Initial position gripper = 0 degrees open in 90 close in 0
}


void loop() {
  // 1. قراءة التردد الأحمر
  digitalWrite(S2, LOW); digitalWrite(S3, LOW);
  int rFreq = pulseIn(sensorOut, LOW);
  delay(10);
  
  // 2. قراءة التردد الأخضر
  digitalWrite(S2, HIGH); digitalWrite(S3, HIGH);
  int gFreq = pulseIn(sensorOut, LOW);
  delay(10);
  
  // 3. قراءة التردد الأزرق
  digitalWrite(S2, LOW); digitalWrite(S3, HIGH);
  int bFreq = pulseIn(sensorOut, LOW);
  delay(10);

  // طباعة القيم عشان تتابعيها في الـ Serial Monitor
  Serial.print("R: "); Serial.print(rFreq);
  Serial.print(" G: "); Serial.print(gFreq);
  Serial.print(" B: "); Serial.print(bFreq);

  // --- منطق تمييز الألوان بناءً على قراءاتك ---

  // حالة الأحمر: لو R هي الأقل وبفرق واضح
  if (rFreq < gFreq && rFreq < bFreq && rFreq < 200) {
    setColor(255, 0, 0); // نوّر أحمر
    Serial.println(" -> Detected: RED");
    redCase();
  } 
  
  // حالة الأخضر: ضفنا هامش (15) عشان ما يتلخبطش مع الأزرق
  else if (gFreq < rFreq && (gFreq < bFreq + 15) && gFreq < 260) {
    setColor(0, 255, 0); // نوّر أخضر
    Serial.println(" -> Detected: GREEN");
    greenCase();
  }

  // حالة الأزرق: لازم يكون أقل من الأخضر بفرق 15 على الأقل
  else if (bFreq < rFreq && bFreq < (gFreq - 15) && bFreq < 200) {
    setColor(0, 0, 255); // نوّر أزرق
    Serial.println(" -> Detected: BLUE");
    blueCase();
  }

  // لو مفيش لون واضح أو القيم عالية (بعيد)
  else {
    setColor(0, 0, 0); // اطفي الـ LED
    Serial.println(" -> No Color");
  }

  delay(200); 

}
void setColor(int r, int g, int b) {
  analogWrite(redPin, b);
  analogWrite(greenPin, g);
  analogWrite(bluePin, r);
}