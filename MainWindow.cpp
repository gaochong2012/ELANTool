//
// Created by gaochong on 17-11-24.
//

#include "MainWindow.h"
#include <QLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QAbstractSocket>
#include <QVariant>
//#include <QOverload>
#include <QStatusBar>

#include <QProgressBar>
#include "GCArp.h"
#include "GCIP.h"
#include "QCTools.h"

#include "GCSQLite.h"
#include "WorkerThread.h"
#include "ListThread.h"


MainWindow::MainWindow() {

   QWidget *mainWidget = new QWidget;
   this->setCentralWidget( mainWidget );

   QVBoxLayout *mainlayout = new QVBoxLayout;
   mainWidget->setLayout( mainlayout );

   QWidget *topWidget = new QWidget;
   QWidget *centerWidget = new QWidget;
   QWidget *bottomWidget = new QWidget;
   mainlayout->addWidget( topWidget );
   mainlayout->addWidget( centerWidget );
   mainlayout->addWidget( bottomWidget );

   QVBoxLayout *centerLayout = new QVBoxLayout;
   centerWidget->setLayout( centerLayout );
   QStringList tableHand;
   tableHand << "IP" << "MAC" << "HOSTNAME" << "Organization";
   lanIPTable = new LanIPTable(  tableHand );
   centerLayout->addWidget( lanIPTable );

   testButton2 =  new QPushButton("列队测试");
   testButton1 =  new QPushButton("极速测试");
   nicList    = new QComboBox;
   sIPEdit =  new QLineEdit;
   dIPEdit =  new QLineEdit;
   initValue();



   QHBoxLayout *topLayout =  new QHBoxLayout;
   topWidget->setLayout( topLayout );
   topLayout->addWidget( nicList );
   topLayout->addWidget( sIPEdit );
   topLayout->addWidget( dIPEdit );
   //topLayout->addWidget( testButton1 );
   topLayout->addWidget( testButton2 );
   topLayout->addStretch();


   QHBoxLayout *bottomLayout =  new QHBoxLayout;
   bottomWidget->setLayout( bottomLayout );
   label1 = new QLabel;
   label2 = new QLabel;
   label3 = new QLabel;
   bottomLayout->addWidget( label1 );
   bottomLayout->addWidget( label2 );
   //bottomLayout->addWidget( label3 );
   bottomLayout->addStretch();
   label1->setText( "探测IP: " );

   progressbar = new QProgressBar;
   statusBar()->addWidget( progressbar,100 );


   oui = new GCSQLite("MACDB2.db");

   setWindowTitle( "以太网活动机器探测" );
   //qRegisterMetaType<QList<QPersistentModelIndex> > ("QList<QPersistentModelIndex>");
   //connect( testButton1,&QPushButton::clicked,this,&MainWindow::TestLan  );
   connect( testButton2,&QPushButton::clicked,this,&MainWindow::TestLan2  );


}



MainWindow::~MainWindow() {

}

QSize MainWindow::sizeHint() const {


    return   QSize( 1024,768 );  //QWidget::sizeHint();
}

void MainWindow::initValue(){

    /**
     * 读取网卡列表 ；MAC
     ===================================================================*/
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();
    //QHostAddress address2;
    QString newMAC = "";
    QString Address = "";
    QString Netmask = "";
    QString Broadcast = "";
    foreach(QNetworkInterface interface,list){

        if ( "lo" == interface.name() ){

            continue;
        }

        qDebug() << "Device:"<<interface.name();
        qDebug() << "mac:"<<interface.hardwareAddress();


        newMAC = interface.hardwareAddress();

        //取 IP 地址
        QList<QNetworkAddressEntry> entryList = interface.addressEntries();

        foreach( QNetworkAddressEntry entry,entryList ){
            QHostAddress address2 = entry.ip();

            if( address2.protocol() == QAbstractSocket::IPv4Protocol ) {

                qDebug()<< "IPV4 Address:" <<entry.ip().toString();
                qDebug()<< "Netmask: "  <<entry.netmask().toString();
                qDebug()<< "Broadcast:" <<entry.broadcast().toString();

                Address = entry.ip().toString();
                Netmask = entry.netmask().toString();
                Broadcast = entry.broadcast().toString();
                nicList->addItem( interface.name() , Address );

                break; //取出第一个IP即可，忽略多IP情况
            }
        }
        //break; //取出第一个网卡即可，忽略多网卡情况
    }



    sIPEdit =  new QLineEdit;
    dIPEdit =  new QLineEdit;

    connect(nicList, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[this](int index){

         qDebug()<< index <<  nicList->currentData().toString();

         QString tip = nicList->currentData().toString();

         sIPEdit->setText( tip.mid(0 , tip.lastIndexOf(".") +1 ) + "1"  );
         dIPEdit->setText( tip.mid(0 , tip.lastIndexOf(".") +1 ) + "254"  );


    });


    nicList->currentIndexChanged( 0 );


}

void MainWindow::TestLan2(){


    if ( ! isInputIP( sIPEdit->text() ) ||  ! isInputIP( dIPEdit->text() ) ) {

        QMessageBox::information( this, "提醒" ,"输入的IP地址有误，请填写正确的IP");
        return;
    }


    this->ktCount = 0;
    this->lanIPTable->ClearAllItem();
    progressbar->setValue( 0 );

    GCIP *testIP = new GCIP;

    /** 启地址　*/
    char sip_pchar[20]; memset( sip_pchar ,0 ,20);
    strcpy( sip_pchar,  QCTools::QStringTOpchar( sIPEdit->text() ));

    /** 止地址　*/
    char dip_pchar[20]; memset( dip_pchar,0 ,20 );
    strcpy( dip_pchar,  QCTools::QStringTOpchar( dIPEdit->text() ));

    /** 地址段总数　*/
    int count = testIP->IP2Count( sip_pchar ,dip_pchar  );

    /** 准备参数　*/
    struct TLanNetList *tlanNetList = new TLanNetList;
    memset( tlanNetList,0 , sizeof( TLanNetList ) );
    strcpy( tlanNetList->Local   , QCTools::QStringTOpchar( nicList->currentText() ) );
    strcpy( tlanNetList->LocalIP , QCTools::QStringTOpchar( nicList->currentData().toString() ) );
    strcpy( tlanNetList->SIP ,sip_pchar );
    strcpy( tlanNetList->DIP ,dip_pchar );

    /** 创建一个新的线程　*/
    ListThread *listThread =  new ListThread( tlanNetList  );
    listThread->start();

    /** 监听线程发送　notify　消息，由主线程更新主界面*/
    connect( listThread ,SIGNAL( notify(LanTableRecord *) ),this,SLOT( OnNotify(LanTableRecord *) ) );


    connect( listThread, &ListThread::notifyNextIP ,this,[=](QString nip){

        this->ktCount++;
        qDebug() << "Thread Over:" << this->ktCount;
        progressbar->setValue(  ktCount * 100 / count  );
        label2->setText( nip );
        if(  count  == ktCount -1 ){
            label2->setText( "结束" );
        }
    });


}


void MainWindow::AppendItem( LanTableRecord* ltRecord  ){

    int nRow = this->lanIPTable->getDataModel()->rowCount();
    this->lanIPTable->getDataModel()->insertRow( nRow );
    this->lanIPTable->getDataModel()->setItem( nRow ,0 , new QStandardItem(  ltRecord->IP ) );
    this->lanIPTable->getDataModel()->setItem( nRow ,1 , new QStandardItem(  ltRecord->MAC ) );
    this->lanIPTable->getDataModel()->setItem( nRow ,2 , new QStandardItem(  ltRecord->HOSTNAME ) );
    this->lanIPTable->getDataModel()->setItem( nRow ,3 , new QStandardItem(  ltRecord->Organization ) );

}


void MainWindow::OnNotify(LanTableRecord *ltRecord) {

    qDebug()  << " ==========================> "  << ltRecord->HOSTNAME;

    ltRecord->Organization = oui->FindMADI( ltRecord->MAC.left(8).replace(":","-") );
    AppendItem( ltRecord );
}


bool MainWindow::isInputIP(  QString IPStr ){

    QRegExp regExp("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");

    if( !regExp.exactMatch( IPStr ) ) {
        return false;
    }
    return true;


}