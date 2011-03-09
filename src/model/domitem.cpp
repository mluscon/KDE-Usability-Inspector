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
