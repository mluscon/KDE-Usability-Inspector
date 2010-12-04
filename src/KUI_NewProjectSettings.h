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

#include "KUI_Settings.h"

#include <KDialog>
#include <KTabWidget>
#include <KLocale>
#include <QList>


class QWidget;
class QString;
class KLineEdit;
class QSpinBox;
class NewProjectSettings;
class NewProject;

class NewUser : public QObject, public User {
  Q_OBJECT
  private:
    QWidget *myWidget;
    NewProjectSettings *settings;
  public:
   NewUser(NewProjectSettings *sett);
   QWidget* getWidget();
  private slots:
    void changeName(QString newName);
    void changeAge(int newAge);
                               
};

class NewProjectSettings : public KDialog, public ProjectSettings {
  Q_OBJECT
  private:
    QList<NewUser*> users;
    KTabWidget *tabWidget;
    QSpinBox *numBox;
    KLineEdit *folderLine;
    
  public:
    NewProjectSettings(QWidget *parent);
    
    void addUser();
    void delUser(QWidget *userWidget);
    QWidget *getTabWidget();
    void changeTabTitle(NewUser *user, QString name);
      
  public slots:
    void folderChangeSlot(QString newFolder);
    void usersCountChangedSlot(int newCount);
    void addUserSlot();
    void closeTabSlot(QWidget *widget);
    void setFolderSlot(); 
};


#endif

