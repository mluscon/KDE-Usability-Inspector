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

NewProject::NewProject(QWidget* parent): KDialog(parent)
{
  
  this->setCaption(i18n("Session settings"));
  
  KTabWidget *settings = new KTabWidget(this);
  this->setMainWidget(settings);
  
  QWidget *session = new QWidget(settings);
  settings->addTab(session, i18n("Session"));
  
  sessionLayout = new QVBoxLayout(settings);
  
  QGridLayout *gridLayout = new QGridLayout();
  sessionLayout->addLayout(gridLayout, Qt::AlignTop);
  gridLayout->addWidget(new QLabel(i18n("Session folder:")), 0, 0);
  
  KLineEdit *folderLine = new KLineEdit(settings);
  gridLayout->addWidget(folderLine, 0, 1);
  
  QToolButton *folderButton = new QToolButton(settings);
  folderButton->setIcon(KIcon("document-open-folder"));
  folderButton->setToolTip(i18n("Select Folder"));
  connect(folderButton, SIGNAL(clicked()), this, SLOT(changeFolderSlot()));
  gridLayout->addWidget(folderButton, 0, 2);
  
  gridLayout->addWidget(new QLabel(i18n("Number of users:")), 1, 0);
  QSpinBox *numBox = new QSpinBox(settings);
  numBox->setMinimum(1);
  this->adduser();
  connect(numBox, SIGNAL(valueChanged(int)), this, SLOT(usersCountChangedSlot(int)));
  gridLayout->addWidget(numBox);
  
  
  
  session->setLayout(sessionLayout);
  
}


void NewProject::adduser()
{
  User *newUser = new User;
  users.append(newUser);

  QGridLayout *gridLayout = new QGridLayout();
  
  gridLayout->addWidget(new QLabel(i18n("Name:")), 0, 0);
  KLineEdit *nameLine = new KLineEdit();
  connect(nameLine, SIGNAL(userTextChanged(QString)), newUser, SLOT(changeName(QString)));
  gridLayout->addWidget(nameLine, 0, 1);
  
  gridLayout->addWidget(new QLabel(i18n("Age:")), 1, 0);
  QSpinBox *ageBox = new QSpinBox();
  ageBox->setMinimum(1);
  ageBox->setMaximum(200);
  connect(ageBox, SIGNAL(valueChanged(int)), newUser, SLOT(changeAge(int)));
  gridLayout->addWidget(ageBox, 1, 1);
  
  actualUsersCount=1;
  
  newUser->myGrid=gridLayout;
  sessionLayout->addLayout(gridLayout);
}

void NewProject::deleteUser(User* delUser )
{
  QList<User*>::iterator iter;

  for ( iter=users.begin(); iter!=users.end(); iter++ ) {
    if (*iter==delUser) {
      sessionLayout->removeWidget(delUser->myGrid);
      delete (delUser);
      users.erase(iter);
      break;
    }
  }
};

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
 
 this->repaint();   
}



void NewProject::changeFolderSlot()
{

}


