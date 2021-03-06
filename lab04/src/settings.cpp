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

//    fillMode = s.value("fillMode", FILL_POINTS).toInt();
//    lightingEnabled = s.value("lightingEnabled", false).toBool();
//    shadingMode = s.value("shadingMode", SHADING_FLAT).toInt();
//    fountainPattern = s.value("fountainPattern", PATTERN_CONTINUOUS).toInt();

//    fillMode = FILL_POINTS;
    lightingEnabled = false;
    shadingMode = SHADING_FLAT;
    fountainPattern = PATTERN_CONTINUOUS;
}

void Settings::saveSettings()
{
    //QSettings s;

    //don't save settings, just use defaults

}
