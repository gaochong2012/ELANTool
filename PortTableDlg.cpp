//
// Created by gaochong on 17-12-4.
//

#include <QtWidgets/QVBoxLayout>
#include "PortTableDlg.h"
#include "PortListViewAll.h"
#include "GCSQLite.h"
#include <QDebug>
#include <QList>
#include <QtWidgets/QLabel>
#include <QPushButton>
#include <QString>
#include <QMessageBox>


#define    DBNAME        "macport.db"

PortTableDlg::PortTableDlg( MainWindow  *mainUI, QWidget *parent ) : QWidget(parent) {


    this->mainUI = mainUI;

    QHBoxLayout *dmainLayout = new QHBoxLayout;

    QVBoxLayout *allportlaoyout =  new QVBoxLayout;
    QVBoxLayout *selectportlaoyout =  new QVBoxLayout;

    QLabel *allPortLabel=  new QLabel("（ 应用 ）端口数据库");
    QLabel *selPortLabel=  new QLabel("已选探测端口");
    allPortLabel2=  new QLabel("记录总数: ");
    selPortLabel2=  new QLabel("已选择: ");


    dmainLayout->addLayout( selectportlaoyout );
    dmainLayout->addLayout( allportlaoyout );

    QStringList tHand;
    tHand << "port" << "Protocol" << "company";
    portListViewAll  = new PortListViewAll( tHand );
    allportlaoyout->addWidget( allPortLabel );
    allportlaoyout->addWidget( portListViewAll );
    allportlaoyout->addWidget( allPortLabel2 );


    SelportListView  = new PortListViewAll( tHand );
    selectportlaoyout->addWidget( selPortLabel );
    selectportlaoyout->addWidget( SelportListView );
    selectportlaoyout->addWidget( selPortLabel2 );

    QList<PortData> selport = mainUI->getSSPort();
    int row = 0;
    foreach( PortData portData , selport ){

        SelportListView->getDataModel()->insertRow( row );
        SelportListView->getDataModel()->setItem( row,0, new QStandardItem(  portData.port )  );
        SelportListView->getDataModel()->setItem( row,1, new QStandardItem(  portData.Protocol )  );
        SelportListView->getDataModel()->setItem( row,2, new QStandardItem(  portData.company )  );
        row++;
    }
    QString text =  QString::number( selport.size(),10);
    selPortLabel2->setText( "已选择: "  +   text  );



    GCSQLite *dbread = new GCSQLite( DBNAME );
    QList<PortData> allport = dbread->ReadALLPort();
    AllPortCount =  allport.size();
    row = 0;
    foreach( PortData allportData , allport ){


      foreach( PortData  selportData, selport ){


          if (  allportData.port == selportData.port && allportData.Protocol == selportData.Protocol   ){

              continue;
          }
     }

       portListViewAll->getDataModel()->insertRow( row );
       portListViewAll->getDataModel()->setItem( row,0, new QStandardItem(  allportData.port )  );
       portListViewAll->getDataModel()->setItem( row,1, new QStandardItem(  allportData.Protocol )  );
       portListViewAll->getDataModel()->setItem( row,2, new QStandardItem(  allportData.company )  );

       row++;
    }

    text =  QString::number( AllPortCount,10)+ " / " + QString::number( AllPortCount,10);
    allPortLabel2->setText( "记录总数: "  +   text  );






    connect(  portListViewAll,
              SIGNAL(doubleClicked(QModelIndex)),
              this,
              SLOT( portListViewAlltemDBClick(QModelIndex)));


    connect(  SelportListView,
              SIGNAL(doubleClicked(QModelIndex)),
              this,
              SLOT( SelportListViewItemDBClick(QModelIndex)));



    QHBoxLayout *buttonGroup = new QHBoxLayout;
    QPushButton *SaveButton = new QPushButton("保存设置");
    QPushButton *CleanButton = new QPushButton("取消");
    buttonGroup->addStretch();
    buttonGroup->addWidget( CleanButton );
    buttonGroup->addWidget( SaveButton  );

    QVBoxLayout *DMainLayout = new QVBoxLayout;
    DMainLayout->addLayout( dmainLayout );
    DMainLayout->addLayout( buttonGroup );

    setLayout( DMainLayout  );


    connect(  SaveButton,
              SIGNAL( clicked() ),
              this,
              SLOT( SaveSelPort()));

    connect(  CleanButton,
              SIGNAL(clicked()),
              this,
              SLOT( CloseSetupDlg()));



    setWindowTitle( "选择需探测的端口" );



}

PortTableDlg::~PortTableDlg() {

}


QSize PortTableDlg::sizeHint() const {


    return QSize( 800,600 );
}

void PortTableDlg::SelportListViewItemDBClick( QModelIndex index ){

    qDebug() << index;

    int row = portListViewAll->getDataModel()->rowCount();
    portListViewAll->getDataModel()->insertRow( row,index );

    portListViewAll->getDataModel()->setItem( row,0 , new QStandardItem( index.model()->index( index.row(),0 ).data().toString().trimmed() ) );
    portListViewAll->getDataModel()->setItem( row,1 , new QStandardItem( index.model()->index( index.row(),1 ).data().toString().trimmed() ) );
    portListViewAll->getDataModel()->setItem( row,2 , new QStandardItem( index.model()->index( index.row(),2 ).data().toString().trimmed() ) );

    SelportListView->getDataModel()->removeRow(  index.row() );

    UpdateRowCount();

}

void PortTableDlg::portListViewAlltemDBClick( QModelIndex index){

    qDebug() << index;

    int row = SelportListView->getDataModel()->rowCount();
    SelportListView->getDataModel()->insertRow( row,index );

    SelportListView->getDataModel()->setItem( row,0 , new QStandardItem( index.model()->index( index.row(),0 ).data().toString().trimmed() ) );
    SelportListView->getDataModel()->setItem( row,1 , new QStandardItem( index.model()->index( index.row(),1 ).data().toString().trimmed() ) );
    SelportListView->getDataModel()->setItem( row,2 , new QStandardItem( index.model()->index( index.row(),2 ).data().toString().trimmed() ) );


    portListViewAll->getDataModel()->removeRow(  index.row() );

    UpdateRowCount();

}

void PortTableDlg::SaveSelPort(){


    int rowCount = SelportListView->getDataModel()->rowCount();

    if ( rowCount > 10 ){

        QMessageBox::warning( this,"提醒","一次探测的端口队例数不要超过 10 个，过多的 SOCKET 连接可能导致系统可用系源不足..."  );
        return;
    }

    QList<PortData> selport;
    for( int i = 0 ; i < SelportListView->getDataModel()->rowCount() ; i++ ){

        PortData portData;

        portData.port = SelportListView->getDataModel()->index( i,0 ).data().toString();
        portData.Protocol =  SelportListView->getDataModel()->index( i,1 ).data().toString();
        portData.company =  SelportListView->getDataModel()->index( i,2 ).data().toString();

        selport.append( portData );
    }

    mainUI->setSSPort(  selport );

    close();

}


void PortTableDlg::CloseSetupDlg(){

    close();
}


void PortTableDlg::UpdateRowCount(){

    int z = AllPortCount - SelportListView->getDataModel()->rowCount();
    QString text =  QString::number( z,10)+ " / " + QString::number( AllPortCount,10);
    allPortLabel2->setText( "记录总数: "  +   text  );

    z = SelportListView->getDataModel()->rowCount();
    text =  QString::number( z,10);
    selPortLabel2->setText( "已选择: "  +   text  );

}


