#include "WiFiAPConfigServer.h"
#include "unity.h"
#include <fakeit.hpp>

WiFiAPConfigServer* configServer;
fakeit::Mock<AsyncWebServerRequest> mockRequest;

void setUp() {
    // Create an instance of WiFiAPConfigServer for each test case
    configServer = new WiFiAPConfigServer("MySSID", "MyPassword");
}

void tearDown() {
    // Clean up resources after each test case
    delete configServer;
}

void test_getDmxRootChannel() {
    // Verify the default root DMX channel
    TEST_ASSERT_EQUAL_INT(1, configServer->getDmxRootChannel());
}

void test_getMode() {
    // Verify the default mode
    TEST_ASSERT_EQUAL_INT(1, configServer->getMode());
}

void test_getIdleTime() {
    // Verify the initial idle time
    TEST_ASSERT_EQUAL_UINT32(0, configServer->getIdleTime());
}

void test_handleConfigUpdate() {
    // Set up mock request parameters
    fakeit::When(Method(mockRequest, hasParam)).Return(true);
    fakeit::When(Method(mockRequest, getParamValue)).Return("5", "2");

    // Call the handleConfigUpdate() function
    configServer->handleConfigUpdate(&mockRequest.get());

    // Add assertions to verify the behavior of handleConfigUpdate()
    // based on the input request and expected output

    // Verify the updated values
    TEST_ASSERT_EQUAL_INT(5, configServer->getDmxRootChannel());
    TEST_ASSERT_EQUAL_INT(2, configServer->getMode());
}

int main() {
    UNITY_BEGIN();

    // Run the setup function before each test case
    RUN_TEST(setUp);

    // Run the test cases
    RUN_TEST(test_getDmxRootChannel);
    RUN_TEST(test_getMode);
    RUN_TEST(test_getIdleTime);
    RUN_TEST(test_handleConfigUpdate);

    // Run the teardown function after each test case
    RUN_TEST(tearDown);

    UNITY_END();
}
