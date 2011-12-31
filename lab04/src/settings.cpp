#include "settings.h"
#include <QFile>
#include <QSettings>

Settings settings;


/**
  Loads the application settings, or, if no saved settings are available,
         loads default values for the settings. You can change the defaults here.
**/
void Settings::loadSettingsOrDefaults()
{
    // Set the default values below
    QSettings s;

    fillMode = s.value("fillMode", FILL_POINTS).toInt();
    lightingEnabled = s.value("lightingEnabled", false).toBool();
    shadingMode = s.value("shadingMode", SHADING_FLAT).toInt();
    fountainPattern = s.value("fountainPattern", PATTERN_CONTINUOUS).toInt();
    useImage = s.value("useImage", true).toBool();
    useTime = s.value("useTime", false).toBool();
}

void Settings::saveSettings()
{
    QSettings s;

    s.setValue("fillMode", fillMode);
    s.setValue("lightingEnabled", lightingEnabled);
    s.setValue("shadingMode", shadingMode);
    s.setValue("fountainPattern", fountainPattern);
    s.setValue("useImage", useImage);
    s.setValue("useTime", useTime);
}
