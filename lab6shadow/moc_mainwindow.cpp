/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created: Thu Dec 22 23:37:10 2011
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../lab06/src/mainwindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_MainWindow[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      18,   12,   11,   11, 0x08,
      51,   12,   11,   11, 0x08,
      84,   12,   11,   11, 0x08,
     115,   12,   11,   11, 0x08,
     151,   12,   11,   11, 0x08,
     182,   11,   11,   11, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_MainWindow[] = {
    "MainWindow\0\0value\0on_speedSlider_valueChanged(int)\0"
    "on_forceSlider_valueChanged(int)\0"
    "on_forceDial_valueChanged(int)\0"
    "on_velocitySlider_valueChanged(int)\0"
    "on_velocityDial_dialMoved(int)\0"
    "initializeGL_Done()\0"
};

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow,
      qt_meta_data_MainWindow, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &MainWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: on_speedSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: on_forceSlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: on_forceDial_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: on_velocitySlider_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: on_velocityDial_dialMoved((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: initializeGL_Done(); break;
        default: ;
        }
        _id -= 6;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
