#include <Arduino.h>
#include "unity.h"
//#include "file_to_test.h"

void setUp(void) {
  // set stuff up here
}

void tearDown(void) {
  // clean stuff up here
}

void test_setup(void) {
    Serial.begin(115200);
    pinMode(2, OUTPUT);
}

void test_blink(void) {
    for(int i=0; i<10; i++){
    digitalWrite(2, HIGH);
    Serial.println("LED is on");
    delay(1000);
    digitalWrite(2, LOW);
    Serial.println("LED is off");
    delay(1000);
    }
}

int runUnityTests(void) {
  UNITY_BEGIN();
  RUN_TEST(test_setup);
  RUN_TEST(test_blink);
  return UNITY_END();
}

void setup() {
  // Wait ~2 seconds before the Unity test runner
  // establishes connection with a board Serial interface
  delay(2000);
  runUnityTests();
}
void loop() {}