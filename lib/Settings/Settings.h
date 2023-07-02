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
    int getBaseChannel() const;

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
    int getMode() const;

private:
    int base_channel_; ///< The base channel.
    int mode_; ///< The mode.

};

#endif  // SETTINGS_H
