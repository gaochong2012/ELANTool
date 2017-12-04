//
// Created by gaochong on 17-12-4.
//

#ifndef ELANTOOL_PORTLISTDLG_H
#define ELANTOOL_PORTLISTDLG_H


#include <QtWidgets/QDialog>
#include <QtCore/QModelIndex>
#include <QtWidgets/QLabel>
#include "PortListViewAll.h"

#include "MainWindow.h"


class PortTableDlg : public QWidget{

    Q_OBJECT


private:

    PortListViewAll *portListViewAll;
    PortListViewAll *SelportListView;

    QLabel *allPortLabel2;
    QLabel *selPortLabel2;

    MainWindow  *mainUI;

    int   AllPortCount = 0;

public:
    PortTableDlg(  MainWindow  *mainUI, QWidget *parent = nullptr );

    virtual ~PortTableDlg();

    QSize sizeHint() const override;

    void UpdateRowCount();

private slots:

    void SelportListViewItemDBClick( QModelIndex index );
    void portListViewAlltemDBClick( QModelIndex index );


    void SaveSelPort();

    void CloseSetupDlg();
};


#endif //ELANTOOL_PORTLISTDLG_H
