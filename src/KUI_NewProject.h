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
#include <QGridLayout>

class QVBoxLayout;
class QString;
class KTabWidget;
class QSpinBox;

class User : public QObject {
  Q_OBJECT
  
  private:
    QString name;
    int age;
    
  public:
    User() { name="user"; age=1; };
    ~User() { delete myWidget; };
    QWidget *myWidget;
    
  public slots:
    void changeName(QString newName) { name=newName; };
    void changeAge(int newAge) { age=newAge; };
};


class NewProject : public KDialog {
  Q_OBJECT
  
  public:
    NewProject(QWidget* parent);
  
  private:
    QSpinBox *numBox;
    KTabWidget *usersTabsWidget;
    QList<User*> users;
    int actualUsersCount;
    void adduser();
    void deleteUser(User*);
    void deleteUser(QWidget*);
    void changeTabTitles();
    
  private slots:
    void changeFolderSlot();
    void usersCountChangedSlot(int);
    void closeTabSlot(QWidget*);
   
};

#endif