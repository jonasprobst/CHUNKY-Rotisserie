#ifndef SETTINGS_INTERFACE_H
#define SETTINGS_INTERFACE_H

/**
 * @brief The SettingsInterface class provides an 
 * interface for accessing and modifying application settings.
 */
class SettingsInterface
{
public:
    /**
     * @brief Sets the base channel.
     *
     * @param baseChannel The base channel value to set.
     */
    virtual void setBaseChannel(int baseChannel) = 0;

    /**
     * @brief Gets the current base channel.
     *
     * @return The current base channel value.
     */
    virtual int getBaseChannel() const = 0;

    /**
     * @brief Sets the mode.
     *
     * @param mode The mode value to set.
     */
    virtual void setMode(int mode) = 0;

    /**
     * @brief Gets the current mode.
     *
     * @return The current mode value.
     */
    virtual int getMode() const = 0;
};

#endif // SETTINGS_INTERFACE_H