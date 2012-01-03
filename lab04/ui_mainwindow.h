/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue Jan 3 12:30:06 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDockWidget>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QRadioButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "glwidget.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    GLWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QDockWidget *settingsDock;
    QWidget *settingsDockContent;
    QVBoxLayout *settingsLayout;
    QWidget *widget;
    QGridLayout *gridLayout;
    QGroupBox *shadingGroupBox;
    QRadioButton *shadingModeFlat;
    QRadioButton *shadingModeSmooth;
    QGroupBox *lightingGroupBox;
    QCheckBox *lightingEnabled;
    QGroupBox *fountainPattern;
    QRadioButton *patternContinuous;
    QRadioButton *patternFan;
    QRadioButton *patternZebra;
    QRadioButton *patternCheckered;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(600, 819);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        centralWidget = new GLWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 600, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        MainWindow->setMenuBar(menuBar);
        settingsDock = new QDockWidget(MainWindow);
        settingsDock->setObjectName(QString::fromUtf8("settingsDock"));
        settingsDock->setMinimumSize(QSize(148, 211));
        settingsDock->setFeatures(QDockWidget::DockWidgetMovable);
        settingsDockContent = new QWidget();
        settingsDockContent->setObjectName(QString::fromUtf8("settingsDockContent"));
        settingsDockContent->setAutoFillBackground(false);
        settingsLayout = new QVBoxLayout(settingsDockContent);
        settingsLayout->setSpacing(6);
        settingsLayout->setContentsMargins(11, 11, 11, 11);
        settingsLayout->setObjectName(QString::fromUtf8("settingsLayout"));
        widget = new QWidget(settingsDockContent);
        widget->setObjectName(QString::fromUtf8("widget"));
        gridLayout = new QGridLayout(widget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        shadingGroupBox = new QGroupBox(widget);
        shadingGroupBox->setObjectName(QString::fromUtf8("shadingGroupBox"));
        shadingGroupBox->setEnabled(false);
        shadingModeFlat = new QRadioButton(shadingGroupBox);
        shadingModeFlat->setObjectName(QString::fromUtf8("shadingModeFlat"));
        shadingModeFlat->setGeometry(QRect(0, 30, 140, 22));
        shadingModeFlat->setChecked(true);
        shadingModeSmooth = new QRadioButton(shadingGroupBox);
        shadingModeSmooth->setObjectName(QString::fromUtf8("shadingModeSmooth"));
        shadingModeSmooth->setGeometry(QRect(0, 60, 140, 22));

        gridLayout->addWidget(shadingGroupBox, 0, 0, 1, 1);

        lightingGroupBox = new QGroupBox(widget);
        lightingGroupBox->setObjectName(QString::fromUtf8("lightingGroupBox"));
        lightingGroupBox->setMinimumSize(QSize(0, 0));
        lightingGroupBox->setMaximumSize(QSize(16777215, 60));
        lightingEnabled = new QCheckBox(lightingGroupBox);
        lightingEnabled->setObjectName(QString::fromUtf8("lightingEnabled"));
        lightingEnabled->setEnabled(false);
        lightingEnabled->setGeometry(QRect(0, 30, 140, 22));

        gridLayout->addWidget(lightingGroupBox, 1, 0, 1, 1);


        settingsLayout->addWidget(widget);

        fountainPattern = new QGroupBox(settingsDockContent);
        fountainPattern->setObjectName(QString::fromUtf8("fountainPattern"));
        fountainPattern->setEnabled(true);
        patternContinuous = new QRadioButton(fountainPattern);
        patternContinuous->setObjectName(QString::fromUtf8("patternContinuous"));
        patternContinuous->setGeometry(QRect(0, 40, 102, 20));
        patternFan = new QRadioButton(fountainPattern);
        patternFan->setObjectName(QString::fromUtf8("patternFan"));
        patternFan->setGeometry(QRect(0, 100, 102, 20));
        patternZebra = new QRadioButton(fountainPattern);
        patternZebra->setObjectName(QString::fromUtf8("patternZebra"));
        patternZebra->setGeometry(QRect(0, 130, 102, 20));
        patternCheckered = new QRadioButton(fountainPattern);
        patternCheckered->setObjectName(QString::fromUtf8("patternCheckered"));
        patternCheckered->setGeometry(QRect(0, 70, 102, 20));

        settingsLayout->addWidget(fountainPattern);

        settingsDock->setWidget(settingsDockContent);
        MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), settingsDock);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionQuit);

        retranslateUi(MainWindow);
        QObject::connect(actionQuit, SIGNAL(triggered()), MainWindow, SLOT(close()));
        QObject::connect(lightingEnabled, SIGNAL(toggled(bool)), shadingGroupBox, SLOT(setEnabled(bool)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "[Lab04] OpenGL II", 0, QApplication::UnicodeUTF8));
        actionQuit->setText(QApplication::translate("MainWindow", "Quit", 0, QApplication::UnicodeUTF8));
        actionQuit->setShortcut(QApplication::translate("MainWindow", "Ctrl+Q", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        shadingGroupBox->setTitle(QApplication::translate("MainWindow", "Shading", 0, QApplication::UnicodeUTF8));
        shadingModeFlat->setText(QApplication::translate("MainWindow", "Flat", 0, QApplication::UnicodeUTF8));
        shadingModeSmooth->setText(QApplication::translate("MainWindow", "Smooth", 0, QApplication::UnicodeUTF8));
        lightingGroupBox->setTitle(QApplication::translate("MainWindow", "Lighting", 0, QApplication::UnicodeUTF8));
        lightingEnabled->setText(QApplication::translate("MainWindow", "Use Lighting", 0, QApplication::UnicodeUTF8));
        fountainPattern->setTitle(QApplication::translate("MainWindow", "Fountain Pattern", 0, QApplication::UnicodeUTF8));
        patternContinuous->setText(QApplication::translate("MainWindow", "Continuous", 0, QApplication::UnicodeUTF8));
        patternFan->setText(QApplication::translate("MainWindow", "Fan", 0, QApplication::UnicodeUTF8));
        patternZebra->setText(QApplication::translate("MainWindow", "Zebra", 0, QApplication::UnicodeUTF8));
        patternCheckered->setText(QApplication::translate("MainWindow", "Checkered", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
