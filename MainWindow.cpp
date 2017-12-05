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
#include <QtCore/QFile>
#include "GCArp.h"
#include "GCIP.h"
#include "QCTools.h"

#include "GCSQLite.h"
#include "WorkerThread.h"
#include "ListThread.h"
#include "PortListView.h"
#include "PortTestThread.h"
#include "PortTableDlg.h"

#define    DBNAME        "macport.db"

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

   QHBoxLayout *centerLayout = new QHBoxLayout;
   centerWidget->setLayout( centerLayout );

   QVBoxLayout *leftLayout = new QVBoxLayout;
   QStringList tableHand;
   tableHand << "IP" << "MAC" << "HOSTNAME" << "Organization";
   lanIPTable = new LanIPTable(  tableHand );
   leftLayout->addWidget( lanIPTable );

   QVBoxLayout *rightLayout = new QVBoxLayout;
   QStringList tableHand2;
   tableHand2 << "Port" << "OPEN";
   portListView = new PortListView(  tableHand2 );
   portListView->setFixedWidth( 250 );
   portListView->setColumnWidth( 0,80 );
   portTestLabel = new QLabel("IP：");
   testPortBtn =  new QPushButton("端口探测");
   QPushButton *portSetupBtn = new QPushButton("端口探测配置");

   rightLayout->addWidget( portListView );
   rightLayout->addWidget( portTestLabel );
   rightLayout->addWidget( testPortBtn );
   rightLayout->addWidget( portSetupBtn );

   centerLayout->addLayout( leftLayout );
   centerLayout->addLayout( rightLayout );

   testButton2 =  new QPushButton("列队测试");
   testButton2->setFixedWidth( 150 );

   //testButton1 =  new QPushButton("极速测试");

   nicList    = new QComboBox;
   QLabel *SIPLabel = new QLabel(  "启始IP:" );
   sIPEdit =  new QLineEdit;
   sIPEdit->setFixedWidth( 120 );
   QLabel *EIPLabel = new QLabel(  "结止IP:" );
   dIPEdit =  new QLineEdit;
   dIPEdit->setFixedWidth( 120 );
   QLabel *srnLabel = new QLabel(  "重试:" );
   srCount = new QComboBox;
   QLabel *otLabel = new QLabel(  "超时:" );
   outTime = new QComboBox;
   QLabel *fPortlabel = new QLabel(  "探测端口:" );
   fPort = new QComboBox;

   initValue();

   QHBoxLayout *topLayout =  new QHBoxLayout;
   topWidget->setLayout( topLayout );
   topLayout->addWidget( nicList );
   topLayout->addWidget( SIPLabel );
   topLayout->addWidget( sIPEdit );
   topLayout->addWidget( EIPLabel );
   topLayout->addWidget( dIPEdit );
   topLayout->addWidget( srnLabel );
   topLayout->addWidget( srCount );
   topLayout->addWidget( otLabel );
   topLayout->addWidget( outTime );
   topLayout->addWidget( fPortlabel );
   topLayout->addWidget( fPort );
   topLayout->addWidget( testButton2 );
   topLayout->addStretch();


   QHBoxLayout *bottomLayout =  new QHBoxLayout;
   bottomWidget->setLayout( bottomLayout );
   label1 = new QLabel;
   label2 = new QLabel;
   label3 = new QLabel;
   bottomLayout->addWidget( label1 );
   bottomLayout->addWidget( label2 );
   bottomLayout->addWidget( label3 );
   bottomLayout->addStretch();
   label1->setText( "探测IP: " );

   progressbar = new QProgressBar;
   statusBar()->addWidget( progressbar,100 );


   QString dbfilename = DBNAME;
   if(  QFile::exists( dbfilename )  ){

       oui = new GCSQLite(  dbfilename );


   }else{
       findON = false;

       QMessageBox::warning( this,"提醒", "数据库文件丢失,请确认当目录下存在"+ dbfilename +" 文件,否则无法查询出设备厂商." );
   }

   setWindowTitle( "以太网活动机器探测" );

   connect( testButton2,&QPushButton::clicked,this,&MainWindow::TestLan2  );

   connect( testPortBtn,&QPushButton::clicked,this,&MainWindow::TestPort  );

   connect( portSetupBtn,  &QPushButton::clicked,this,&MainWindow::PortSetup  );

   connect(  lanIPTable,
             SIGNAL(doubleClicked(QModelIndex)),
             this,
             SLOT( LanIPTableItemDBClick(QModelIndex)));

   connect(  portListView,
             SIGNAL(doubleClicked(QModelIndex)),
             this,
             SLOT( PortListViewItemDBClick(QModelIndex)));


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
    QString newMAC = "";
    QString Address = "";
    QString Netmask = "";
    QString Broadcast = "";
    foreach(QNetworkInterface interface,list){

        if ( "lo" == interface.name() ){

            continue;
        }
/**
        qDebug() << "Device:"<<interface.name();
        qDebug() << "mac:"<<interface.hardwareAddress();
*/

        newMAC = interface.hardwareAddress();

        //取 IP 地址
        QList<QNetworkAddressEntry> entryList = interface.addressEntries();

        foreach( QNetworkAddressEntry entry,entryList ){
            QHostAddress address2 = entry.ip();

            if( address2.protocol() == QAbstractSocket::IPv4Protocol ) {
/**
                qDebug()<< "IPV4 Address:" <<entry.ip().toString();
                qDebug()<< "Netmask: "  <<entry.netmask().toString();
                qDebug()<< "Broadcast:" <<entry.broadcast().toString();
*/
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

    /** 超时  */
    outTime->addItem( "50","50" );
    outTime->addItem( "200","200" );
    outTime->addItem( "500","500" );
    outTime->addItem( "1000","1000" );
    outTime->addItem( "2000","2000" );
    outTime->setCurrentIndex(1);
    outTime->currentIndexChanged( 1 );

    /** 重试 */
    srCount->addItem( "1" ,"1" );
    srCount->addItem( "2" ,"2" );
    srCount->addItem( "3" ,"2" );
    srCount->addItem( "4" ,"3" );
    srCount->setCurrentIndex(0);
    srCount->currentIndexChanged( 0 );

    fPort->addItem( "不进行" ,"-9999" );
    fPort->addItem( "22" ,"22" );
    fPort->addItem( "80" ,"80" );
    fPort->addItem( "3389" ,"3389" );
    fPort->setCurrentIndex(0);
    fPort->currentIndexChanged( 0 );

    nicList->setCurrentIndex(0);
    nicList->currentIndexChanged(0);
/**
    connect( outTime, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[this](int index){

        qDebug()<< index <<  outTime->currentData().toString();
    });

    connect( srCount, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[this](int index){

        qDebug()<< index <<  srCount->currentData().toString();
    });


    connect( fPort, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),[this](int index){

        qDebug()<< index <<  fPort->currentData().toString();
    });
*/
}

void MainWindow::TestLan2(){


    if ( ! isInputIP( sIPEdit->text() ) ||  ! isInputIP( dIPEdit->text() ) ) {

        QMessageBox::information( this, "提醒" ,"输入的IP地址有误，请填写正确的IP");
        return;
    }
    label2->setText("");
    label3->setText("");
    this->ktCount = 0;
    this->okCount = 0;
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

    int srCount_i  = srCount->currentData().toInt();
    int outTime_i  = outTime->currentData().toInt();

    int fPort_i = fPort->currentData().toInt();

    /** 准备参数　*/
    struct TLanNetList *tlanNetList = new TLanNetList;
    memset( tlanNetList,0 , sizeof( TLanNetList ) );
    strcpy( tlanNetList->Local   , QCTools::QStringTOpchar( nicList->currentText() ) );
    strcpy( tlanNetList->LocalIP , QCTools::QStringTOpchar( nicList->currentData().toString() ) );
    strcpy( tlanNetList->SIP ,sip_pchar );
    strcpy( tlanNetList->DIP ,dip_pchar );

    /** 创建一个新的线程　*/
    ListThread *listThread = new ListThread( tlanNetList , srCount_i , outTime_i ,fPort_i );
    listThread->start();

    /** 监听线程发送　notify　消息，由主线程更新主界面*/
    connect( listThread ,SIGNAL( notify(LanTableRecord *) ),this,SLOT( OnNotify(LanTableRecord *) ) );


    connect( listThread, &ListThread::notifyNextIP ,this,[=](QString nip){

        this->ktCount++;
        qDebug() << "Thread Over:" << this->ktCount;
        progressbar->setValue(  ktCount * 100 / count  );
        label2->setText( nip );
        if(  count  == ktCount-1  ){
            label2->setText( "结束" );
            label3->setText( "共探测到 "+ QString::number( okCount ) +" 台设备" );
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

void MainWindow::OnNotifyPortTest( int port , int type ) {

    int nRow = this->portListView->getDataModel()->rowCount();

    this->portListView->getDataModel()->insertRow( nRow );
    this->portListView->getDataModel()->setItem( nRow ,0 , new QStandardItem( QString::number( port ) ) );

    if( type == 0 ){

        this->portListView->getDataModel()->setItem( nRow ,1 , new QStandardItem( "开启"  ) );

    }else{

        this->portListView->getDataModel()->setItem( nRow ,1 , new QStandardItem(  "未启用" ) );
    }

}

void MainWindow::OnNotify(LanTableRecord *ltRecord) {

    ++okCount;
    if ( findON && oui != NULL   ){

        ltRecord->Organization = oui->FindMADI( ltRecord->MAC.left(8).replace( ":","-" ) );
    }

    AppendItem( ltRecord );
}


bool MainWindow::isInputIP(  QString IPStr ){

    QRegExp regExp("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");

    if( !regExp.exactMatch( IPStr ) ) {
        return false;
    }
    return true;

}

//
void MainWindow::TestPort(){

    if ( this->getSSPort().size() == 0   ){

        QMessageBox::warning( this,"提醒","未选择需要探测的端口列表,请先行配置.");
        return;
    }

    this->portListView->ClearAllItem();

    if( this->MMIP.length() != 0 ){

        qDebug()<< this->MMIP;
        char tIP[20];
        memset( tIP,0,sizeof( tIP )*sizeof( char ) );
        strcpy( tIP, QCTools::QStringTOpchar( MMIP ));

        PortTestThread *connectPort = new PortTestThread( tIP , this->getSSPort() );
        connectPort->start();

        connect( connectPort  ,SIGNAL( OnNotifyPortTest(int,int) ),this,SLOT( OnNotifyPortTest( int,int ) ) );
    }

}

void MainWindow::PortSetup(){

    PortTableDlg *psDlg =  new PortTableDlg( this );
    psDlg->show();

}



void MainWindow::LanIPTableItemDBClick( QModelIndex modelIndex){

    this->MMIP = modelIndex.model()->index( modelIndex.row(),0 ).data().toString().trimmed();
    portTestLabel->setText( "IP：" + this->MMIP );
}

void MainWindow::PortListViewItemDBClick( QModelIndex modelIndex ){

    //qDebug()<< modelIndex;


}

void MainWindow::setSSPort(QList<PortData> SSPort) {

    this->SSPort =  SSPort;

}

QList<PortData> MainWindow::getSSPort() {

    return QList<PortData>( this->SSPort );
}
