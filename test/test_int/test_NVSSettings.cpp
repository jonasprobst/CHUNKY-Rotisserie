#include <unity.h>
#include <Arduino.h>
#include <Preferences.h>
#include <NVSSettings.h>

void setUp() {
    // Delete all preferences before each test to ensure a clean slate
    Preferences preferences;
    preferences.begin("my-app", false);
    preferences.clear();
    preferences.end();
}

void test_load_values() {
    // Save some test values to preferences
    Preferences preferences;
    preferences.begin("my-app", false);
    preferences.putUInt("validKey", 123456789);
    preferences.putUInt("rootChannel", 123);
    preferences.putUInt("mode", 456);
    preferences.end();

    // Check that the settings class loads these values correctly
    NVSSettings settings;
    TEST_ASSERT_EQUAL_INT(123, settings.getRootChannel());
    TEST_ASSERT_EQUAL_INT(456, settings.getMode());
}

void test_save_values() {
    NVSSettings settings;
    settings.save(123, 456);

    // Check that the values were saved correctly to preferences
    Preferences preferences;
    preferences.begin("my-app", false);
    unsigned int rootChannel = preferences.getUInt("rootChannel", 0);
    unsigned int mode = preferences.getUInt("mode", 0);
    preferences.end();

    TEST_ASSERT_EQUAL_INT(123, rootChannel);
    TEST_ASSERT_EQUAL_INT(456, mode);
}

void setup() {
    delay(2000); // Allow some time to open the serial monitor after a reset
    UNITY_BEGIN();
    RUN_TEST(test_load_values);
    RUN_TEST(test_save_values);
    UNITY_END();
}

void loop() {
}
