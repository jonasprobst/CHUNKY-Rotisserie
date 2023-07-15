#ifndef SETTINGS_H
#define SETTINGS_H

#include "SettingsInterface.h"
#include "NVSStorage.h"
#include <set>

// TODO: Make this a singleton?

/**
 * @brief The Settings class encapsulates the runtime settings of the application.
 *
 * @note Base channel is the DMX start address (aka offset) for this device and
 *       its consecutive channels. The base channel can be set to 1 – 505.
 *
 *       Mode is the operation mode of this device. Mode 1-3 are DMX modes, 7-8 are manual modes.
 *       // DMX Modes (DMX required)
 *       1 = Slow acceleration and deceleration (ramp) when changing speed or direction
 *       2 = Normal acceleration and deceleration (ramp) when changing speed or direction
 *       3 = Fast acceleration and deceleration (ramp) when changing speed or direction
 *       // Manual Modes (no DMX)
 *       7 = Clockwise (CW) rotation with speed set by base channel. 255 = max speed.
 *       8 = Counter clockwise (CCW) rotation with speed set by base channel. 255 = max speed.
 *       0,4,5,6,9 = No function -> motor is off.
 *
 *       When in Mode 1-3, Channel 6 determines the operation mode:
 *       0-50% Rotation mode
 *       51-54% Setup limits mode (mandatory setup for position and angular mode)
 *       55-79% Position mode
 *       80-100% Angular mode
 *
 *       Rotation mode:
 *       - Channel 3: speed (0 = slow, 255 = fast)
 *       - Channel 4: Clockwise (CW) rotation (0 = off, 255 = on)
 *       - Channel 5: Counter clockwise (CCW) rotation (0 = off, 255 = on)
 *
 *        Setup limits mode:
 *        1. set channel 6 to 51-54% to enable setup limits mode
 *        2. rotate motor with Channel 4 to desired position
 *        3. set channel 4 to 0 to save position as CW limit
 *        4. rotate motor with Channel 5 to desired position
 *        5. set channel 5 to 0 to save position as CCW limit
 *        6. continue with position or angular mode
 *   
 *        Position mode:
 *        1. Follow the steps in Setup limits mode
 *        2. Set channel 6 to 55-79% to enable position mode
 *        3. Set channel 3 desired speed (0 = slow, 255 = fast)
 *        4. Set channel 1 (rough) and 2 (fine) to move the motor 
 *           to the relative position. The motor will turn in the direction
 *           that is the shortest way to the target position.
 * 
 *        Angular mode:
 *        1. Follow the steps in Setup limits mode
 *        2. Set channel 6 to 80-100% to enable angular mode
 *        3. Set channel 3 desired speed (0 = slow, 255 = fast)
 *        4. Set channel 1 (rough) and 2 (fine) to move the motor
 * 
 */
class Settings : public SettingsInterface
{
public:
    /**
     * @brief Constructs a Settings object.
     */
    Settings();

    /**
     * @brief Destroys a Settings object.
     */
    ~Settings();

    /**
     * @brief Sets the base channel. Valid values are 1 – 505.
     *
     * @param base_channel The base channel value to set.
     */
    bool SetBaseChannel(uint16_t base_channel);

    /**
     * @brief Gets the current base channel.
     *
     * @return The current base channel value.
     */
    uint16_t GetBaseChannel() const;

    /**
     * @brief Sets the mode. Valid values are 1,2,3,7,8.
     *
     * @param mode The mode value to set.
     */
    bool SetMode(uint8_t mode);

    /**
     * @brief Gets the current mode.
     *
     * @return The current mode value.
     */
    uint8_t GetMode() const;

private:
    const std::set<uint8_t> valid_modes_ = {1, 2, 3, 7, 8}; // valid modes
    uint8_t mode_;            // operation mode of this device
    uint16_t base_channel_;   // DMX Start Address aka offset
    NVSStorage *nvs_storage_; // NVS storage pointer
    
};

#endif // SETTINGS_H
