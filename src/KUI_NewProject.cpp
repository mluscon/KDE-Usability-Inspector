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

#include "KUI_NewProject.h"

#include <QWidget>
#include <KLocale>
#include <QApplication>
#include <KTabWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <KLineEdit>
#include <QSpinBox>
#include <QToolButton>
#include <QString>
#include <QToolButton>

NewProject::NewProject(QWidget* parent): KDialog(parent)
{
  
  this->setCaption(i18n("Session settings"));
  
  QWidget *centralWidget = new QWidget(this);
  QVBoxLayout *centralWidgetLayout = new QVBoxLayout();
  
  QGridLayout *gridLayout = new QGridLayout();
  centralWidgetLayout->addLayout(gridLayout, Qt::AlignTop);
 
  gridLayout->addWidget(new QLabel(i18n("Session folder:")), 0, 0);
  KLineEdit *folderLine = new KLineEdit();
  gridLayout->addWidget(folderLine, 0, 1);
  
  gridLayout->addWidget(new QLabel(i18n("Number of users:")), 1, 0);
  numBox = new QSpinBox();
  numBox->setMinimum(1);
  actualUsersCount=0;
  connect(numBox, SIGNAL(valueChanged(int)), this, SLOT(usersCountChangedSlot(int)));
  gridLayout->addWidget(numBox, 1, 1);
  
  
  usersTabsWidget = new KTabWidget(this);
  centralWidgetLayout->addWidget(usersTabsWidget);
  usersTabsWidget->setCloseButtonEnabled(true);
  connect(usersTabsWidget, SIGNAL(closeRequest(QWidget*)), this, SLOT(closeTabSlot(QWidget*)));
  this->adduser();
  
  centralWidget->setLayout(centralWidgetLayout);
  this->setMainWidget(centralWidget);
  
}


void NewProject::adduser()
{
  User *newUser = new User;
  users.append(newUser);
  
  newUser->myWidget = new QWidget(this);
  QGridLayout *gridLayout = new QGridLayout(newUser->myWidget);
  
  gridLayout->addWidget(new QLabel(i18n("Name:"), newUser->myWidget), 0, 0);
  KLineEdit *nameLine = new KLineEdit(newUser->myWidget);
  connect(nameLine, SIGNAL(userTextChanged(QString)), newUser, SLOT(changeName(QString)));
  gridLayout->addWidget(nameLine, 0, 1);
  
  gridLayout->addWidget(new QLabel(i18n("Age:"), newUser->myWidget), 1, 0);
  QSpinBox *ageBox = new QSpinBox(newUser->myWidget);
  ageBox->setMinimum(1);
  ageBox->setMaximum(200);
  connect(ageBox, SIGNAL(valueChanged(int)), newUser, SLOT(changeAge(int)));
  gridLayout->addWidget(ageBox, 1, 1);
  
  actualUsersCount++;
  newUser->myWidget->setLayout(gridLayout);
    
  usersTabsWidget->addTab(newUser->myWidget, QString("User").append(QString::number(actualUsersCount)));
}

void NewProject::deleteUser(User* delUser )
{
  QList<User*>::iterator iter;

  for ( iter=users.begin(); iter!=users.end(); iter++ ) {
    if (*iter==delUser) {
     
      delete (delUser);
      users.erase(iter);
      actualUsersCount--;
      changeTabTitles();
      break;
    }
  }
};

void NewProject::deleteUser(QWidget* delUserWidget )
{
  QList<User*>::iterator iter;

  for ( iter=users.begin(); iter!=users.end(); iter++ ) {
    if ((*iter)->myWidget==delUserWidget) {
      
      delete (delUserWidget);
      users.erase(iter);
      actualUsersCount--;
      numBox->setValue(actualUsersCount);
      changeTabTitles();
      break;
    }
  }
};

void NewProject::changeTabTitles()
{
  for (int i=0; i<=users.count(); i++) {
    usersTabsWidget->setTabText(i, QString("User").append(QString::number(i+1)));
  }

}

void NewProject::closeTabSlot(QWidget* delUserWidget)
{
  if (actualUsersCount>1) {
    deleteUser(delUserWidget);
  }
}





void NewProject::usersCountChangedSlot(int newCount)
{
  
 if (newCount>actualUsersCount) {
   for ( int i=actualUsersCount ; i<newCount; ++i ) {
     this->adduser();
   }
 }
 
  if (newCount<actualUsersCount) {
   for ( int i=actualUsersCount ; i>newCount; --i ) {
     this->deleteUser(*(--users.end()));
   }
 }
 
 this->update();   
}

void NewProject::changeFolderSlot()
{

}


