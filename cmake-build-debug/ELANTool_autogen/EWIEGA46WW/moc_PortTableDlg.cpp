/****************************************************************************
** Meta object code from reading C++ file 'PortTableDlg.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../PortTableDlg.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'PortTableDlg.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_PortTableDlg_t {
    QByteArrayData data[7];
    char stringdata0[99];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PortTableDlg_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PortTableDlg_t qt_meta_stringdata_PortTableDlg = {
    {
QT_MOC_LITERAL(0, 0, 12), // "PortTableDlg"
QT_MOC_LITERAL(1, 13, 26), // "SelportListViewItemDBClick"
QT_MOC_LITERAL(2, 40, 0), // ""
QT_MOC_LITERAL(3, 41, 5), // "index"
QT_MOC_LITERAL(4, 47, 25), // "portListViewAlltemDBClick"
QT_MOC_LITERAL(5, 73, 11), // "SaveSelPort"
QT_MOC_LITERAL(6, 85, 13) // "CloseSetupDlg"

    },
    "PortTableDlg\0SelportListViewItemDBClick\0"
    "\0index\0portListViewAlltemDBClick\0"
    "SaveSelPort\0CloseSetupDlg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PortTableDlg[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x08 /* Private */,
       4,    1,   37,    2, 0x08 /* Private */,
       5,    0,   40,    2, 0x08 /* Private */,
       6,    0,   41,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QModelIndex,    3,
    QMetaType::Void, QMetaType::QModelIndex,    3,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void PortTableDlg::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        PortTableDlg *_t = static_cast<PortTableDlg *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SelportListViewItemDBClick((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 1: _t->portListViewAlltemDBClick((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 2: _t->SaveSelPort(); break;
        case 3: _t->CloseSetupDlg(); break;
        default: ;
        }
    }
}

const QMetaObject PortTableDlg::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_PortTableDlg.data,
      qt_meta_data_PortTableDlg,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *PortTableDlg::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PortTableDlg::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_PortTableDlg.stringdata0))
        return static_cast<void*>(const_cast< PortTableDlg*>(this));
    return QWidget::qt_metacast(_clname);
}

int PortTableDlg::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
