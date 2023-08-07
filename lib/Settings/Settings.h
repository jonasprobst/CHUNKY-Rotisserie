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
 *       its consecutive channels: The base channel can be 0 – 505, First channel
 *       is the base channel + 1, second channel is the base channel + 2, etc.
 *
 *       Modes:
 *       // DMX Modes (DMX channels 1-6 are used)
 *       1 = Slow Ramp -> acceleration and deceleration when changing speed or direction (delta v)
 *       2 = Normal Ramp
 *       3 = Fast Ramp
 *       // Manual Modes (no DMX)
 *       7 = Clockwise (CW) Rotation, speed set by base channel.
 *       8 = Counter clockwise (CCW) Rotation, speed set by base channel.
 *       0,4,5,6,9 = No function -> motor is off.
 *
 *       Channels:
 *       1 = Position rough (High byte of 16bit)
 *       2 = Position fine (Low byte of 16bit)
 *       3 = Set the maximum speed the motor will ramp up to
 *       4 = Rotate CW relative to max speed (and set CW limit)
 *       5 = Rotate CCW relative to max speed (and set CCW limit)
 *       6 = Motor Operation Mode (only relevant in Mode 1-3, details see below)
 *           - 0-50% Continuous rotation mode
 *           - 51-54% Position Mode (set limits enabled)
 *           - 55-79% Position mode
 *           - 80-100% Angular mode
 *
 *       Continuous rotation mode:
 *       The motor will rotate continuously in the direction set by channel 4 or 5 at the speed set by channel 3.
 *       - Channel 3: Set max speed
 *       - Channel 4: Rotate CW relative to max speed. Channel 5 must be 0.
 *       - Channel 5: Rotate CCW relative to max speed. Channel 4 must be 0.
 *
 *       Position mode (limits are lost when powered off):
 *       The Position is relative to the set limits. E.g. 25% = 25% of the distance between the limits.
 *       Setup (has to be done once each power cycle):
 *         1. Set all channels to 0.
 *         2. Set channel 6 (motor mode) to 51-54% to set limits
 *         3. Set channel 3 (max speed) to 50% (slow rotation)
 *         4. Rotate motor with Channel 4 (CW) to desired position
 *         5. Set channel 4 to 0 to save position as CW limit
 *         6. Rotate motor with Channel 5 (CCW) to desired position
 *         7. Set channel 5 to 0 to save position as CCW limit
 *         8. Set channel 6 to 55-79% to enable position mode
 *       Controls:
 *         - Channel 1 + 2: Set target position relative to the set limits
 *         - Channel 3: Set Max speed
 *
 *       Angular mode (zero position is lost when powered off):
 *       The Position is relative to a full rotation. E.g. 25% = 90°, 50% = 180°, etc.
 *       The motor will always take the shortest path to the target position and choose its direction accordingly,
 *       unless it is already moving in that direction (e.g. switching from continuous rotation -> example below)
 *       Setup (has to be done once each power cycle):
 *         1. Set all channels to 0.
 *         2. Set channel 3 (max speed) to 50% (slow rotation)
 *         3. Rotate motor with Channel 4 (CW) at least one full turn to home it (find its zero position)
 *         4. Set channel 4 (CW) to 0.
 *         5. Set channel 6 to 80-100% to enable angular mode.
 *       Controls:
 *         - Channel 1 + 2: Set target position relative to a full rotation (360°)
 *         - Channel 3: Set max speed
 *       Example switching from continuous rotation to angular mode:
 *         − Normal: The current position is 5° and the next set position is 350°.
 *           → The rotator will move 15° back to the new position.
 *         - Exception: The rotator is moving continuously (channel 4 or 5 set), and the next position is 350°
 *           → The rotator will continue in the same direction until it reaches the new position even if going backwards would be shorter.
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
    // TODO: Move valid modes to a more generic place?
    const std::set<uint8_t> valid_modes_ = {1, 2, 3, 7, 8}; // valid modes for this device
    uint8_t mode_;                                          // operation mode of this device
    uint16_t base_channel_;                                 // DMX Start Address aka offset
    NVSStorage *nvs_storage_;                               // NVS storage pointer
};

#endif // SETTINGS_H
