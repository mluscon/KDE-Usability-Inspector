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

#include "KUI_NewProjectSettings.h"

#include <KLocale>
#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <KLineEdit>
#include <QLabel>
#include <QToolButton>
#include <KFileDialog>
#include <KUrl>
#include <QSpinBox>


//NewUser
NewUser::NewUser(NewProjectSettings* sett): QObject(sett)
{
  settings=sett;
  myWidget = new QWidget;
      
  QGridLayout *gridLayout = new QGridLayout();
  gridLayout->addWidget(new QLabel(i18n("Name:")), 0, 0);
  KLineEdit *nameLine = new KLineEdit();
  nameLine->setText(i18n("New User"));
  connect(nameLine, SIGNAL(userTextChanged(QString)), this, SLOT(changeName(QString)));
  gridLayout->addWidget(nameLine, 0, 1);
  
  gridLayout->addWidget(new QLabel(i18n("Age:")), 1, 0);
  QSpinBox *ageBox = new QSpinBox();
  ageBox->setMinimum(1);
  ageBox->setMaximum(200);
  connect(ageBox, SIGNAL(valueChanged(int)), this, SLOT(changeAge(int)));
  gridLayout->addWidget(ageBox, 1, 1);
  
  QVBoxLayout *vBoxLayuot = new QVBoxLayout;
  vBoxLayuot->addLayout(gridLayout, Qt::AlignTop);
 
  myWidget->setLayout(vBoxLayuot);
}


QWidget* NewUser::getWidget()
{
   return myWidget;
}

void NewUser::changeName(QString newName)
{
  name = newName;
  settings->changeTabTitle(this, name);
}

void NewUser::changeAge(int newAge)
{
  age = newAge;
}

//NewProjectSettings

NewProjectSettings::NewProjectSettings(QWidget* parent): KDialog(parent)
{
  
  this->setCaption(i18n("Session settings"));
  
  QVBoxLayout *centralWidgetLayout = new QVBoxLayout();
  
  QGridLayout *gridLayout = new QGridLayout();
  centralWidgetLayout->addLayout(gridLayout, Qt::AlignTop);
  gridLayout->addWidget(new QLabel(i18n("Session folder:")), 0, 0);
  folderLine = new KLineEdit();
  folderLine->setText(projectFolder);
  connect(folderLine, SIGNAL(textEdited(QString)), this, SLOT(folderChangeSlot(QString)));
  gridLayout->addWidget(folderLine, 0, 1);
  QToolButton *setFolder = new QToolButton;
  setFolder->setIcon(KIcon("document-open-folder"));
  connect(setFolder, SIGNAL(released()), this, SLOT(setFolderSlot()));
  gridLayout->addWidget(setFolder, 0, 2);
  
  
  gridLayout->addWidget(new QLabel(i18n("Number of users:")), 1, 0);
  numBox = new QSpinBox();
  numBox->setMinimum(1);
  connect(numBox, SIGNAL(valueChanged(int)), this, SLOT(usersCountChangedSlot(int)));
  gridLayout->addWidget(numBox, 1, 1);
  QToolButton *addUserButton = new QToolButton;
  addUserButton->setIcon(KIcon("list-add"));
  connect(addUserButton, SIGNAL(clicked(bool)), this, SLOT(addUserSlot()));
  gridLayout->addWidget(addUserButton);
  
  tabWidget = new KTabWidget(parent);
  tabWidget->setCloseButtonEnabled(true);
  connect(tabWidget, SIGNAL(closeRequest(QWidget*)), this, SLOT(closeTabSlot(QWidget*)));
  
  centralWidgetLayout->addWidget(tabWidget);
  addUser();  
  
  QWidget *centralWidget = new QWidget(this);
  centralWidget->setLayout(centralWidgetLayout);
  this->setMainWidget(centralWidget);
   
}

void NewProjectSettings::addUser()
{
  NewUser *user = new NewUser(this);
  users.append(user);
  tabWidget->addTab(user->getWidget(), user->getName());
  usersCount++;
}

void NewProjectSettings::delUser(QWidget* userWidget)
{
  if (usersCount>1) {
    QList<NewUser*>::iterator iter;
    for( iter=users.begin(); iter<users.end(); ++iter) {
      if ((*iter)->getWidget() == userWidget) {
      tabWidget->removeTab(users.indexOf(*iter));
      delete *iter;
      users.erase(iter);
      --usersCount;
      }
    }
  }
}

QWidget* NewProjectSettings::getTabWidget()
{
  return tabWidget;
}

void NewProjectSettings::changeTabTitle(NewUser* user, QString name)
{
  tabWidget->setTabText(users.indexOf(user), name);
}

void NewProjectSettings::folderChangeSlot(QString newFolder)
{
  projectFolder = newFolder;
}

void NewProjectSettings::usersCountChangedSlot(int newCount)
{
   if (newCount>usersCount) {
     for (int i=usersCount; i<=newCount; ++i) {
       addUser();
     }
   }
   if (newCount<usersCount) {
    for (int i=newCount; i<=usersCount; ++i) {
      delUser(users.last()->getWidget());
    }
  }
}

void NewProjectSettings::addUserSlot()
{
  addUser();
  numBox->setValue(usersCount);
}


void NewProjectSettings::closeTabSlot(QWidget* widget)
{
  delUser(widget);
  numBox->setValue(usersCount);
}


void NewProjectSettings::setFolderSlot()
{
  KUrl url;
  url.setDirectory(projectFolder);
  QString path = KFileDialog::getExistingDirectory(url, this, QString("KUI_project"));

  if (path.isEmpty()) {
    return;
  }
  
  projectFolder=path;
  folderLine->setText(projectFolder);  
}





