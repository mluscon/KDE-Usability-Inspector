/***************************************************************************************
* Copyright (c) 2010 Michal Luscon <mluscon@gmail.com>                                 *
*                                                                                      *
* This program is free software; you can redistribute it and/or modify it under        *
* the terms of the GNU General Public License as published by the Free Software        *
* Foundation; either version 2 of the License, or (at your option) any later           *
* version.                                                                             *
*                                                                                      *
* This program is distributed in the hope that it will be useful, but WITHOUT ANY      *
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A      *
* PARTICULAR PURPOSE. See the GNU General Public License for more details.             *
*                                                                                      *
* You should have received a copy of the GNU General Public License along with         *
* this program.  If not, see <http://www.gnu.org/licenses/>.                           *
****************************************************************************************/




#include <QtGui>
#include <QtXml>
#include <KMessageBox>
#include <KLocale>

#include "domitem.h"
#include "dommodel.h"

DomModel::DomModel(QString path , QObject *parent) : QAbstractItemModel(parent)
{
  domFile = new QFile( path );
  if ( !domFile->open(QIODevice::ReadOnly) ) {
    KMessageBox::error( 0, i18n("Error1"), i18n("Error"));
    return;
  }
  
  
  
  domDocument.setContent( domFile );
  domFile->close();
  
  rootItem = new DomItem(domDocument, 0); 
}

DomModel::~DomModel()
{
  if ( !rootItem )
    delete rootItem;
  
  if ( !domFile )
    delete domFile;  
}

int DomModel::columnCount(const QModelIndex &/*parent*/) const
{
  return 3;
}

QVariant DomModel::data(const QModelIndex &index, int role) const
{
     if (!index.isValid())
         return QVariant();

     if (role != Qt::DisplayRole)
         return QVariant();

     DomItem *item = static_cast<DomItem*>(index.internalPointer());

     QDomNode node = item->node();
     QStringList attributes;
     QDomNamedNodeMap attributeMap = node.attributes();

     switch (index.column()) {
         
         case 0: if ( node.toElement().hasAttribute( "name" ) ) {
                    return node.toElement().attribute( "name", "empty");
                 } else {
                    return node.nodeName();
                 }
         
         case 1: if ( node.toElement().hasAttribute( "screen" ) ) {
                    return node.toElement().attribute( "screen", "empty");
                 } else {
                    return QString( "empty" );
                 }
           
             
         case 2: if ( node.toElement().hasAttribute( "camera" ) ) {
                    return node.toElement().attribute( "camera", "empty");
                 } else {
                    return QString( "empty" );
                 }
            
         default:
             return QVariant();
     }
 }

Qt::ItemFlags DomModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

 QVariant DomModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
 {
     if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
         switch (section) {
             case 0:
                 return tr("Name");
             case 1:
                 return tr("Attributes");
             case 2:
                 return tr("Value");
             default:
                 return QVariant();
         }
     }

     return QVariant();
 }

 QModelIndex DomModel::index(int row, int column, const QModelIndex &parent)
             const
 {
     if (!hasIndex(row, column, parent))
         return QModelIndex();

     DomItem *parentItem;

     if (!parent.isValid())
         parentItem = rootItem;
     else
         parentItem = static_cast<DomItem*>(parent.internalPointer());

     DomItem *childItem = parentItem->child(row);
     if (childItem)
         return createIndex(row, column, childItem);
     else
         return QModelIndex();
 }

 QModelIndex DomModel::parent(const QModelIndex &child) const
 {
     if (!child.isValid())
         return QModelIndex();

     DomItem *childItem = static_cast<DomItem*>(child.internalPointer());
     DomItem *parentItem = childItem->parent();

     if (!parentItem || parentItem == rootItem)
         return QModelIndex();

     return createIndex(parentItem->row(), 0, parentItem);
 }

int DomModel::rowCount(const QModelIndex &parent) const
{
     if (parent.column() > 0)
         return 0;

     DomItem *parentItem;

     if (!parent.isValid())
         parentItem = rootItem;
     else
         parentItem = static_cast<DomItem*>(parent.internalPointer());

     return parentItem->node().childNodes().count();
}

bool DomModel::setData(const QModelIndex &index, const QVariant &value, int role) 
{
  if ( !index.isValid() )
    return false;
  
  if ( role != Qt::EditRole )
    return false;
  
  DomItem *item = static_cast<DomItem*>( index.internalPointer() );
  
  switch ( index.column() ) {
    
    case 0: item->node().toElement().setAttribute("name", value.toString());
    break;
    
    case 1: item->node().toElement().setAttribute("camera", value.toString() );
    break;
    
    case 2: item->node().toElement().setAttribute("screen", value.toString() );
    break;
    
  }
  
  if ( domFile->exists() ) {
    if ( !domFile->remove() ) {
     return false;
    }
  }
  
  if ( !domFile->open(QIODevice::WriteOnly) ) {
    KMessageBox::error( 0, i18n("Error1"), i18n("Error"));
    return false;
  }
  
  QTextStream out( domFile );
  domDocument.save(out, 4);
  domFile->close();
  
  emit dataChanged(index, index);
  
  qDebug() << "data changed";
  return true;  
}

bool DomModel::insertRows(int position, int rows, const QModelIndex& parent)
{
  DomItem *parentItem;
  if ( !parent.isValid() ) {
    return false;
  } else {
    parentItem = static_cast<DomItem*> ( parent.internalPointer() );
  }
  
  if ( parentItem->node().nodeName() == "session" ) {
    
    QDomElement newNode = domDocument.createElement( "user" );
  
    newNode.setAttribute( "name", "New User");
    parentItem->node().appendChild( newNode );
  
    qDebug() << "parent row: " << parentItem->row();
    DomItem *newItem = new DomItem( newNode, parentItem->row()+1, parentItem);
  
    beginInsertRows( parent, parentItem->row()+1, parentItem->row()+1 );
    parentItem->appendChild( newItem );
    endInsertRows();
  
    if ( !domFile->open(QFile::WriteOnly) ) {
      return false;
    }
  
    QTextStream out( domFile );
    domDocument.save(out, 4);
    domFile->close();
    
    return true;
  }
  
  if ( parentItem->node().nodeName() == "user" ) {
    
    QDomElement newNode = domDocument.createElement( "video" );
  
    newNode.setAttribute( "screen", "empty");
    newNode.setAttribute( "camera", "empty");
    parentItem->node().appendChild( newNode );
  
    qDebug() << "parent row: " << parentItem->row();
    DomItem *newItem = new DomItem( newNode, parentItem->row()+1, parentItem);
  
    beginInsertRows( parent, parentItem->row()+1, parentItem->row()+1 );
    parentItem->appendChild( newItem );
    endInsertRows();
  
    if ( !domFile->open(QFile::WriteOnly) ) {
      return false;
    }
  
    QTextStream out( domFile );
    domDocument.save(out, 4);
    domFile->close();
    
    return true;
  }  
  
  
  return false;
}

bool DomModel::removeRows(int position, int rows, const QModelIndex& parent)
{
   DomItem *parentItem;
  
  if ( !parent.isValid() ) {
    parentItem =rootItem;
  } else {
    parentItem = static_cast<DomItem*> ( parent.internalPointer() );
  }
  
  qDebug() << "removing: " << position << " " << rows << " parent: " << parentItem->node().nodeName();
   
  beginRemoveRows( parent, position, position+rows);
  parentItem->removeChild( position );
  endRemoveRows();
  
  if ( !domFile->open(QFile::WriteOnly) ) {
    return false;
  }
  
  QTextStream out( domFile );
  domDocument.save(out, 4);
  domFile->close();
  
  return true;
}

