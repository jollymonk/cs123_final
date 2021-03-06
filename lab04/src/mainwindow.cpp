#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <assert.h>
#include "databinding.h"
#include "settings.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    settings.loadSettingsOrDefaults();
    m_ui->setupUi(this);

    // Restore the UI settings
    QSettings qtSettings("Lab04");
    restoreGeometry(qtSettings.value("geometry").toByteArray());
    restoreState(qtSettings.value("windowState").toByteArray());

    dataBind();
}

MainWindow::~MainWindow()
{
    foreach (DataBinding *b, m_bindings)
    {
        delete b;
    }
    delete m_ui;
}

void MainWindow::dataBind()
{
#define BIND(b) { DataBinding *_b = (b); m_bindings.push_back(_b); assert(connect(_b, SIGNAL(dataChanged()), this, SLOT(settingsChanged()))); }

//    BIND(ChoiceBinding::bindRadioButtons(NUM_FILL_MODES, settings.fillMode,
//                                    m_ui->fillModePoints,
//                                    m_ui->fillModeWireframe,
//                                    m_ui->fillModeShaded));
    BIND(ChoiceBinding::bindRadioButtons(NUM_SHADING_MODES, settings.shadingMode,
                                    m_ui->shadingModeFlat,
                                    m_ui->shadingModeSmooth));
    BIND(ChoiceBinding::bindRadioButtons(NUM_PATTERNS, settings.fountainPattern,
                                    m_ui->patternContinuous,
                                    m_ui->patternCheckered,
                                    m_ui->patternFan,
                                    m_ui->patternZebra));
    BIND(BoolBinding::bindCheckbox(m_ui->lightingEnabled, settings.lightingEnabled));

#undef BIND
}

void MainWindow::settingsChanged()
{
    m_ui->centralWidget->updateSettings();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Save the settings before we quit
    settings.saveSettings();
    QSettings qtSettings("Lab04");
    qtSettings.setValue("geometry", saveGeometry());
    qtSettings.setValue("windowState", saveState());

    QMainWindow::closeEvent(event);
}

void MainWindow::fileOpen()
{
    QString file = QFileDialog::getOpenFileName(this, QString(), "/Users/mjunck/Dev/cs123/");
    if (!file.isNull())
    {

//        if (m_ui->centralWidget->loadImage(file))
//            QMessageBox::critical(this, "Error", "Could not load image \"" + file + "\"");
//        else
//        {
//            //image successfully loaded
//        }
    }
}
