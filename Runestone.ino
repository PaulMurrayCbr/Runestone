#include <DebounceInput.h>

//const int rgb1[] = {11, 10, 9};
//const int rgb2[] = {3, 6, 5};
const int rgb1[] = {11, 10, 9};
const int rgb2[] = {6, 5, 3};

const int SLIDER_A = 7;
const int SLIDER_B = 8;
const int DR_WHO = 12;

const byte BLACK[] = {0, 0, 0};
const byte RED[] = {255, 0, 0};
const byte GREEN[] = {0, 255, 0};
const byte BLUE[] = {0, 0, 255};
const byte CYAN[] = {0, 255 , 255};
const byte MAGENTA[] = {255, 0, 255};
const byte YELLOW[] = {255, 255, 0};
const byte WHITE[] = {40, 120, 255};
const byte DIM[] = {30, 110, 128};

const byte drWhoA[] = {0, 0, 0};
const byte drWhoB[] = {0, 128, 255};

//const byte redgoldA[] = {255, 255, 0};
//const byte redgoldB[] = {32, 0, 0};
const byte redgoldA[] = {255, 255, 8};
const byte redgoldB[] = {32, 16, 8};

const byte corruptedA[] = {64, 0, 192};
const byte corruptedB[] = {0, 128, 16};

const byte bluegreenA[] = {0, 64, 192};
const byte bluegreenB[] = {0, 128, 16};

unsigned long ts;

void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(rgb1[i], OUTPUT);
    pinMode(rgb2[i], OUTPUT);
  }

  pinMode(SLIDER_A, INPUT_PULLUP);
  pinMode(SLIDER_B, INPUT_PULLUP);
  pinMode(DR_WHO, INPUT_PULLUP);


  pinMode(13, OUTPUT);

  clear();
  
//  Serial.begin(9600);
}

void c2pin(const byte c[], const int pin[]) {
  for (int i = 0; i < 3; i++) analogWrite(pin[i], c[i]);
}

void cc2pin(const byte c1[], const byte c2[], float f, const int pin[]) {
  for (int i = 0; i < 3; i++) analogWrite(pin[i], c1[i] + (((int)c2[i] - (int)c1[i])*f));
}

void clear() {
  c2pin(BLACK, rgb1);
  c2pin(BLACK, rgb2);
  digitalWrite(13, false);

  ts = ~millis();
}

void loop() {
  if (millis() == ts) return;
  ts = millis();

  boolean sliderA = !digitalRead(SLIDER_A);
  boolean sliderB = !digitalRead(SLIDER_B);
  boolean drWho = !digitalRead(DR_WHO);

//  Serial.print("A: ");
//  Serial.print(sliderA);
//  Serial.print(", B: ");
//  Serial.print(sliderB);
//  Serial.print(", drWho: ");
//  Serial.print(drWho);
//  Serial.println();
  

  if (drWho) {
    dr_who();
    digitalWrite(13, false);
  }
  else if (sliderA && sliderB) {
    // test_pattern();
    blue_green();
    digitalWrite(13, true);
  }
  else if (!sliderA && !sliderB) {
    off();
    digitalWrite(13, false);
  }
  else if (sliderA) {
    red_gold();
    digitalWrite(13, false);
  }
  else {
    corrupted();
    digitalWrite(13, false);
  }
}

float cycle(int ms, float offs) {
  int tts = (millis() + (long)(ms * offs)) % ms;

  return tts / (float)ms;
}

float cycle2(int ms, float offs) {
  float foo = cycle(ms, offs);
  if (foo < .5) {
    return foo * 2;
  }
  else {
    return 2 - (foo * 2);
  }
}

void off() {
  c2pin(BLACK, rgb1);
  c2pin(BLACK, rgb2);
}

void dr_who() {
  cc2pin(drWhoB, drWhoA, cycle(125, 0), rgb1);
  cc2pin(drWhoB, drWhoA, cycle(125, 0), rgb2);
}

void test_pattern() {
  cc2pin(WHITE, DIM, cycle2(1000, 0), rgb1);
  cc2pin(WHITE, DIM, cycle2(1618, 0), rgb2);
}

boolean redgold_flash = false;
byte redgold_flash_1 = 0;
byte redgold_flash_2 = 0;
unsigned long redgold_flash_ms;


void red_gold() {
  if (redgold_flash) {
    if (millis() - redgold_flash_ms >= 1000) {
      redgold_flash = false;
    }
    else {
      if (random(100) == 0) redgold_flash_1 = random(5);
      if (random(100) == 0) redgold_flash_2 = random(5);

      c2pin(redgold_flash_1 == 0 ? BLUE : BLACK, rgb1);
      c2pin(redgold_flash_2 ==0 ? BLUE : BLACK, rgb2);
    }
  }
  else {
    if (random(1000) == 0 && random(120) == 0)
    {
      redgold_flash = true;
      redgold_flash_ms = millis();
    }
    else {
      cc2pin(redgoldA, redgoldB, cycle2(10000, 0), rgb1);
      cc2pin(redgoldA, redgoldB, cycle2(16180, .5), rgb2);
    }
  }
}

void blue_green() {
  if (redgold_flash) {
    if (millis() - redgold_flash_ms >= 1000) {
      redgold_flash = false;
    }
    else {
      if (random(100) == 0) redgold_flash_1 = random(5);
      if (random(100) == 0) redgold_flash_2 = random(5);

      c2pin(redgold_flash_1 == 0 ? RED : BLACK, rgb1);
      c2pin(redgold_flash_2 ==0 ? RED : BLACK, rgb2);
    }
  }
  else {
    if (random(1000) == 0 && random(120) == 0)
    {
      redgold_flash = true;
      redgold_flash_ms = millis();
    }
    else {
      cc2pin(bluegreenA, bluegreenB, cycle2(10000, 0), rgb1);
      cc2pin(bluegreenA, bluegreenB, cycle2(16180, .5), rgb2);
    }
  }
}

void corrupted() {
  if (redgold_flash) {
    if (millis() - redgold_flash_ms >= 2000) {
      redgold_flash = false;
    }
    else {
      if (random(100) == 0) redgold_flash_1 = random(5);
      if (random(100) == 0) redgold_flash_2 = random(5);

      c2pin(redgold_flash_1 == 0 ? RED : BLACK, rgb1);
      c2pin(redgold_flash_2 == 0 ? RED : BLACK, rgb2);
    }
  }
  else {
    if (random(1000) == 0 && random(120) == 0)
    {
      redgold_flash = true;
      redgold_flash_ms = millis();
    }
    else {
      cc2pin(corruptedA, corruptedB, cycle2(10000, 0), rgb1);
      cc2pin(corruptedA, corruptedB, cycle2(16180, .5), rgb2);
    }
  }
}


void aloop() {
  // put your main code here, to run repeatedly:


  double cycle1 = (sin((double)millis() / 2000 * 2 * PI) + 1) / 2;
  double cycle2 = (sin((double)millis() / 1616 * 2 * PI) + 1) / 2;



  analogWrite(rgb1[0], 128 + 64 + 32 - cycle1 * 32);
  analogWrite(rgb1[1], 128 + 64 + cycle1 * 64);
  analogWrite(rgb2[0], 128 + 64 + 32 - cycle2 * 32);
  analogWrite(rgb2[1], 128 + 64 + cycle2 * 64);

  if (random(1000) == 0 && random(20) == 0) {
    analogWrite(rgb1[0], 0);
    analogWrite(rgb2[0], 0);
    analogWrite(rgb1[1], 0);
    analogWrite(rgb2[1], 0);
    for (int i = 0; i < 4; i++)
      analogWrite(rgb1[2], 255);
    analogWrite(rgb2[2], 255);
    delay(125);
    analogWrite(rgb1[2], 0);
    analogWrite(rgb2[2], 0);
    delay(125);
  }

}
