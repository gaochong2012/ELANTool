//
// Created by gaochong on 17-11-24.
//

#ifndef ELANTOOL_MAINWINDOW_H
#define ELANTOOL_MAINWINDOW_H

#include <QMainWindow>
#include "LanIPTable.h"
#include "TableRecord.h"
#include "GCSQLite.h"
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
    QProgressBar *progressbar;
    QLabel       *label1;
    QLabel       *label2;
    QLabel       *label3;
    void AppendItem(LanTableRecord* ltRecord);
    bool isInputIP(QString IPStr);

private slots:

    void initValue();

    void OnNotify( LanTableRecord *ltRecord  );


    void TestLan2();


};


#endif //ELANTOOL_MAINWINDOW_H
