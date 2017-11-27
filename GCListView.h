//
// Created by gaochong on 17-11-24.
//

#ifndef ELANTOOL_GCLISTVIEW_H
#define ELANTOOL_GCLISTVIEW_H

#include <QHeaderView>
#include <QTableView>
#include <QStandardItemModel>
#include <QList>

class GCListView :public QTableView{

Q_OBJECT

public:

    GCListView(  QStringList TableHeader );
    ~GCListView();

    QStandardItemModel* getDataModel();
    void  setDataModel( QStandardItemModel* DataModel );
    void ClearAllItem();

private:

    QStandardItemModel *dataModel;
    QHeaderView *tableHead;

private slots:

    void view_sort(int column);


};



#endif //ELANTOOL_GCLISTVIEW_H
