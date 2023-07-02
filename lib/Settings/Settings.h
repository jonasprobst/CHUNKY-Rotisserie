#ifndef SETTINGS_H
#define SETTINGS_H

/**
 * @brief The Settings class encapsulates the runtime settings of the application.
 *
 */
class Settings {
public:
    /**
     * @brief Sets the base channel.
     *
     * @param base_channel The base channel value to set.
     */
    void setBaseChannel(int base_channel);

    /**
     * @brief Gets the current base channel.
     *
     * @return The current base channel value.
     */
    uint16_t getBaseChannel() const;

    /**
     * @brief Sets the mode.
     *
     * @param mode The mode value to set.
     */
    void setMode(int mode);

    /**
     * @brief Gets the current mode.
     *
     * @return The current mode value.
     */
    uint8_t getMode() const;

private:
    uint16_t base_channel_; ///< The base channel.
    uint8_t mode_; ///< The mode.

};

#endif  // SETTINGS_H
