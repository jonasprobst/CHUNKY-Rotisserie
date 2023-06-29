#include "NVSSettingsInterface.h"

// Mocks the NVCSettings class with dummy storage and no NVC for testing

class MockSettings : public NVSSettingsInterface {
private:
    uint16_t _baseChannel = 1;
    uint16_t _mode = 1;

public:
    void initialize() override {}

    uint16_t getBaseChannel() const override {
        return _baseChannel;
    }

    uint16_t getMode() const override {
        return _mode;
    }

    void save(uint16_t baseChannel, uint16_t mode) override {
        _baseChannel = baseChannel;
        _mode = mode;
    }
};
