#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

#define NUM_EMITTERS 100
#define DROP_SPEED 4
#define FTN_WIDTH 4
#define FTN_HEIGHT 10
#define FTN_TOP 1.5
#define FTN_BOTTOM -1
#define FTN_LEFT -2
#define FRAMES_PER_SEC 30.0

// Enumeration values for the fill modes from which the user can choose in the GUI.
enum FillMode
{
    FILL_POINTS,
    FILL_WIREFRAME,
    FILL_SHADED,
    NUM_FILL_MODES
};

// Enumeration values for the Shapes that the user can select in the GUI.
enum ShadingMode
{
    SHADING_FLAT,
    SHADING_SMOOTH,
    NUM_SHADING_MODES
};


/**

    @struct Settings
    @brief  Stores application settings for the CS123 GUI.

        You can access all app settings through the "settings" global variable.
    The settings will be automatically updated when things are changed in the
    GUI (the reverse is not true however: changing the value of a setting does
    not update the GUI).

*/
struct Settings
{
    // Loads settings from disk, or fills in default values if no saved settings exist.
    void loadSettingsOrDefaults();

    // Saves the current settings to disk.
    void saveSettings();

    int fillMode;      // The drawing mode used to draw primitives.
    bool lightingEnabled;      // Enable or disable lighting.
    int shadingMode;    // The shading mode to be used.
};

// The global Settings object, will be initialized by MainWindow
extern Settings settings;

#endif // SETTINGS_H
