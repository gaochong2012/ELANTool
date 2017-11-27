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

class MainWindow :public QMainWindow{

    Q_OBJECT

public:

    MainWindow();
    ~MainWindow();


protected:
    QSize sizeHint() const override;

private:

    int  ktCount = 0;

    GCSQLite     *oui;

    QPushButton  *testButton1;
    QComboBox    *nicList;
    QLineEdit    *sIPEdit;
    QLineEdit    *dIPEdit;
    LanIPTable  *lanIPTable;
    QProgressBar *progressbar;
    void AppendItem(LanTableRecord* ltRecord);

private slots:

    void Test();
    void initValue();
    void TestLan();

    void OnNotify( LanTableRecord *ltRecord  );
};


#endif //ELANTOOL_MAINWINDOW_H
