//
// Created by gaochong on 17-11-24.
//

#include "GCListView.h"

GCListView::GCListView( QStringList TableHeader ) {

    //整行选择
    this->setSelectionBehavior( QAbstractItemView::SelectRows );
    //不允许编辑
    this->setEditTriggers( QAbstractItemView::NoEditTriggers );

    //表头字体
    //this->horizontalHeader()->setFont(QFont ("Simsun" , 12));

    //设置行交替颜色
    this->setAlternatingRowColors(true);

    //只能单选一个项
    //this->setSelectionMode(QAbstractItemView::SingleSelection);

    //表头信息显示居左
    //this->horizontalHeader()->setDefaultAlignment( Qt::AlignLeft );

    //隐藏垂直表头
    this->verticalHeader()->setVisible( true );

    //隐藏行头
    //this->verticalHeader()->hide();

    //背景网格线设置
    this->setShowGrid(false);

    //网格背景画笔
    this->setGridStyle( Qt::DotLine );

    dataModel = new QStandardItemModel();

    //设置标题
    //m_dataModel->setHeaderData(0,Qt::Horizontal,"标题");

    QStringList tbHeader = TableHeader;

    //设置列数
    dataModel->setColumnCount ( tbHeader.count() );

    //设置表头
    dataModel->setHorizontalHeaderLabels( tbHeader );

    this->setModel(  dataModel );

    // 列宽自适应大小
    //this->horizontalHeader()->setSectionResizeMode( QHeaderView::ResizeToContents );

    //初始列宽定义
    this->setColumnWidth( 0 ,200 );
    this->setColumnWidth( 1 ,200 );
    this->setColumnWidth( 2 ,250 );
    this->setColumnWidth( 3 ,1000 );

    //隐藏列
    //this->setColumnHidden( 5,true );

    //允许右键表格
    this->setContextMenuPolicy( Qt::CustomContextMenu );


    tableHead  = this->horizontalHeader();
    tableHead->setSortIndicator(0,Qt::AscendingOrder);
    tableHead->setSortIndicatorShown(true);
    connect( tableHead, SIGNAL(sectionClicked(int)), this, SLOT(view_sort(int)));



/**
    //右键激活菜单
    connect( this,
             SIGNAL( customContextMenuRequested( QPoint )),
             this,
             SLOT( ShowContextMenu( QPoint )) );
*/

/**

    connect( this,
             SIGNAL(clicked(QModelIndex)),
             this,
             SLOT(  Itemlick(QModelIndex) ));


    connect( this,
             SIGNAL(doubleClicked(QModelIndex)),
             this,
             SLOT( ItemDBClick(QModelIndex)));

**/

/**
     * 初始化右键菜单
*/
    //m_ContextMenu = new QMenu();

}



GCListView::~GCListView() {


    delete dataModel;
}

void GCListView::ClearAllItem(){

    dataModel->removeRows(0, dataModel->rowCount() );

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void GCListView::view_sort(int column) {

    int i   = tableHead->sortIndicatorOrder();

    if( 0 == i ) {

        dataModel->sort(column, Qt::AscendingOrder);

    }else {

        dataModel->sort(column, Qt::DescendingOrder);
    }

}

QStandardItemModel* GCListView::getDataModel() {

    return this->dataModel;
}

void GCListView::setDataModel(QStandardItemModel *DataModel) {

    this->dataModel = DataModel;

}
