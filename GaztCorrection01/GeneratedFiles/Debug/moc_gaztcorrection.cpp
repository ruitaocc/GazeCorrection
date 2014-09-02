/****************************************************************************
** Meta object code from reading C++ file 'gaztcorrection.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../gaztcorrection.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gaztcorrection.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GaztCorrection_t {
    QByteArrayData data[19];
    char stringdata[202];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GaztCorrection_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GaztCorrection_t qt_meta_stringdata_GaztCorrection = {
    {
QT_MOC_LITERAL(0, 0, 14),
QT_MOC_LITERAL(1, 15, 8),
QT_MOC_LITERAL(2, 24, 0),
QT_MOC_LITERAL(3, 25, 15),
QT_MOC_LITERAL(4, 41, 1),
QT_MOC_LITERAL(5, 43, 1),
QT_MOC_LITERAL(6, 45, 9),
QT_MOC_LITERAL(7, 55, 9),
QT_MOC_LITERAL(8, 65, 14),
QT_MOC_LITERAL(9, 80, 4),
QT_MOC_LITERAL(10, 85, 11),
QT_MOC_LITERAL(11, 97, 20),
QT_MOC_LITERAL(12, 118, 9),
QT_MOC_LITERAL(13, 128, 3),
QT_MOC_LITERAL(14, 132, 22),
QT_MOC_LITERAL(15, 155, 8),
QT_MOC_LITERAL(16, 164, 15),
QT_MOC_LITERAL(17, 180, 9),
QT_MOC_LITERAL(18, 190, 11)
    },
    "GaztCorrection\0sendData\0\0vector<CvPoint>\0"
    "a\0b\0playVideo\0stopVideo\0terminateVideo\0"
    "file\0processMode\0setNextFrameToOutput\0"
    "IplImage*\0img\0receiveTerminateSignal\0"
    "dde_init\0dde_first_track\0dde_track\0"
    "calibration"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GaztCorrection[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   74,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   79,    2, 0x0a /* Public */,
       7,    0,   80,    2, 0x0a /* Public */,
       8,    0,   81,    2, 0x0a /* Public */,
       9,    0,   82,    2, 0x0a /* Public */,
      10,    0,   83,    2, 0x0a /* Public */,
      11,    1,   84,    2, 0x0a /* Public */,
      14,    0,   87,    2, 0x0a /* Public */,
      15,    0,   88,    2, 0x0a /* Public */,
      16,    0,   89,    2, 0x0a /* Public */,
      17,    0,   90,    2, 0x0a /* Public */,
      18,    0,   91,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, 0x80000000 | 3,    4,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 12,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void GaztCorrection::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GaztCorrection *_t = static_cast<GaztCorrection *>(_o);
        switch (_id) {
        case 0: _t->sendData((*reinterpret_cast< vector<CvPoint>(*)>(_a[1])),(*reinterpret_cast< vector<CvPoint>(*)>(_a[2]))); break;
        case 1: _t->playVideo(); break;
        case 2: _t->stopVideo(); break;
        case 3: _t->terminateVideo(); break;
        case 4: _t->file(); break;
        case 5: _t->processMode(); break;
        case 6: _t->setNextFrameToOutput((*reinterpret_cast< IplImage*(*)>(_a[1]))); break;
        case 7: _t->receiveTerminateSignal(); break;
        case 8: _t->dde_init(); break;
        case 9: _t->dde_first_track(); break;
        case 10: _t->dde_track(); break;
        case 11: _t->calibration(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GaztCorrection::*_t)(vector<CvPoint> , vector<CvPoint> );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GaztCorrection::sendData)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject GaztCorrection::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_GaztCorrection.data,
      qt_meta_data_GaztCorrection,  qt_static_metacall, 0, 0}
};


const QMetaObject *GaztCorrection::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GaztCorrection::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GaztCorrection.stringdata))
        return static_cast<void*>(const_cast< GaztCorrection*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int GaztCorrection::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void GaztCorrection::sendData(vector<CvPoint> _t1, vector<CvPoint> _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
