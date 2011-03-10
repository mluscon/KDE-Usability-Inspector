/****************************************************************************************
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

#ifndef KUI_NEWPROJECT_H
#define KUI_NEWPROJECT_H

#include <KDialog>
#include <QList>
#include <QItemSelection>


class DomModel;
class KUIModel;
class QString;
class QDataWidgetMapper;
class KLineEdit;
class QTreeView;
class QTreeView;


class NewProjectDialog : public KDialog 
{
  Q_OBJECT
  
public:
  NewProjectDialog(QWidget* );
private:
  KLineEdit *folderLine;
  KLineEdit *nameLine;
  QString sessionFolder;
  QString sessionName;
  
private slots:
  void setSessionFolder();
  void nameChanged(QString newName);
  void folderChanged(QString newFolder);
  void okButtonSlot();

signals:
  void newProjectEnd(QString);  
};


class UsersEditationDialog : public KDialog 
{
  Q_OBJECT
  
public:
  UsersEditationDialog(QWidget* , QString);
private:
  QString path;
  DomModel *model;
  QTreeView *list;
  QDataWidgetMapper *mapper;
  KLineEdit *nameLine;
private slots:
  void addUser();
  void removeUser();
  void okButtonSlot();
  
  
public slots:
  void updateMappers(QItemSelection first, QItemSelection last);

signals:
  void userEditationComplete(QString );
  
};


#endif
