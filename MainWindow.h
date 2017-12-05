//
// Created by gaochong on 17-11-24.
//

#ifndef ELANTOOL_MAINWINDOW_H
#define ELANTOOL_MAINWINDOW_H

#include <QMainWindow>
#include "LanIPListView.h"
#include "TableRecord.h"
#include "GCSQLite.h"
#include "PortListView.h"
#include <QPushButton>
#include <QProgressBar>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>

class MainWindow :public QMainWindow{

    Q_OBJECT

public:

    MainWindow();
    ~MainWindow();


protected:
    QSize sizeHint() const override;

private:

    int  ktCount = 0;
    int  okCount = 0;
    GCSQLite     *oui;

    QPushButton  *testButton1;
    QPushButton  *testButton2;
    QComboBox    *nicList;
    QLineEdit    *sIPEdit;
    QLineEdit    *dIPEdit;
    LanIPTable   *lanIPTable;
    PortListView *portListView;
    QProgressBar *progressbar;
    QLabel       *label1;
    QLabel       *label2;
    QLabel       *label3;
    QComboBox    *srCount;
    QComboBox    *outTime;
    QComboBox    *fPort;

    QLabel       *portTestLabel;
    QPushButton  *testPortBtn;

    void AppendItem(LanTableRecord* ltRecord);
    bool isInputIP(QString IPStr);

    QString           MMIP;
    QList<PortData>   SSPort;

private:
    bool  findON  = true;     //是否通过 MAC 查找厂商



    void initValue();

public:
    QList<PortData> getSSPort();
    void setSSPort( QList<PortData> SSPort );

private slots:



    void OnNotify( LanTableRecord *ltRecord  );
    void OnNotifyPortTest( int port , int type );

    void TestLan2();
    void TestPort();

    void PortListViewItemDBClick(QModelIndex modelIndex);
    void LanIPTableItemDBClick(QModelIndex modelIndex);


    void PortSetup();
};


#endif //ELANTOOL_MAINWINDOW_H
