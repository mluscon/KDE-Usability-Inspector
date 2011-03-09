#ifndef DOMMODEL_H
#define DOMMODEL_H

#include <QAbstractItemModel>
#include <QDomDocument>
#include <QModelIndex>
#include <QVariant>
#include <QFile>

class DomItem;

class DomModel : public QAbstractItemModel
{
  Q_OBJECT

public:
  DomModel(QString, QObject *parent = 0);
  ~DomModel();
  
  QVariant data(const QModelIndex &index, int role) const;
  Qt::ItemFlags flags(const QModelIndex &index) const;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const;
  QModelIndex parent(const QModelIndex &child) const;
  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;
  
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole);
  bool insertRows(int position, int rows,
                  const QModelIndex &parent = QModelIndex());  
  bool removeRows(int position, int rows,
                  const QModelIndex &parent = QModelIndex());
private:
  QFile *domFile;
  QDomDocument domDocument;
  DomItem *rootItem;
};

 #endif