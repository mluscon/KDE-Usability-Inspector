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


#ifndef DOMITEM_H
#define DOMITEM_H

#include <QDomNode>
#include <QHash>

 class DomItem
 {
 public:
     DomItem(QDomNode &node, int row, DomItem *parent = 0);
     ~DomItem();
     DomItem *child(int i);
     DomItem *parent();
     QDomNode node() const;
     int row();
     bool appendChild( DomItem* );
     bool removeChild( int );
 private:
     QDomNode domNode;
     QList<DomItem*> childItems;
     DomItem *parentItem;
     int rowNumber;
 };

 #endif