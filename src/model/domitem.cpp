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


#include <QtXml>

 #include "domitem.h"

 DomItem::DomItem(QDomNode &node, int row, DomItem *parent)
 {
     domNode = node;
     // Record the item's location within its parent.
     rowNumber = row;
     parentItem = parent;
 }

 DomItem::~DomItem()
 {/*
     QList<int,DomItem*>::iterator it;
     for (it = childItems.begin(); it != childItems.end(); ++it)
         delete it.value();
     */
 }

 QDomNode DomItem::node() const
 {
     return domNode;
 }

DomItem *DomItem::parent()
{
  return parentItem;
}

DomItem *DomItem::child(int i)
{
  if ( childItems.length() > i )
    return childItems.at(i);

  if (i >= 0 && i < domNode.childNodes().count()) {
    qDebug() << "child";
    QDomNode childNode = domNode.childNodes().item(i);
    if ( childNode.isNull() )
      return 0;
    DomItem *childItem = new DomItem(childNode, i, this);
    childItems.insert(i, childItem);
    return childItem;
  }
  return 0;
 }

 int DomItem::row()
 {
     return rowNumber;
 }
 
bool DomItem::appendChild(DomItem *child)
{
  //childItems.insert( childItems.count(), child );
  childItems.append( child );
  rowNumber++;
  return true;
}

bool DomItem::removeChild( int i)
{
  
  if ( i >= childItems.count() )
    return false;
  
  if ( childItems.count() <= 0 || rowNumber == 0 )
    return false;
  
  
  node().removeChild( childItems.at( i )->node() );
  childItems.removeAt( i );
  
  rowNumber--;
  
  return true;
}
