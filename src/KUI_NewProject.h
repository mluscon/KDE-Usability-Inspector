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


class QGridLayout;
class QVBoxLayout;
class QString;

class User : public QObject {
  Q_OBJECT
  
  private:
    QString name;
    int age;
    
  public:
    User() { name="user"; age=1; };
    QGridLayout *myGrid;
    
  public slots:
    void changeName(QString newName) { name=newName; };
    void changeAge(int newAge) { age=newAge; };
};


class NewProject : public KDialog {
  Q_OBJECT
  
  public:
    NewProject(QWidget* parent);
  
  private:
    QList<User*> users;
    int actualUsersCount;
    QVBoxLayout *sessionLayout;
    void adduser();
    void deleteUser(User*);
  
  private slots:
    void changeFolderSlot();
    void usersCountChangedSlot(int);
 
};

#endif