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


#include <QtGui>
#include <QFileDialog>
#include <QPixmap>
#include <QApplication>
#include <QString>
#include <cstring>

#include "KUI_project.h"
#include "KUI_record.h"

KUI_project::KUI_project(QWidget* parent): QWidget(parent)
{
  
  setWindowTitle(tr("KDE Usability Inspector"));
  resize(320,240);
  path.push_back("/home/");
  
  
  /*QWidget *screenShotLabel = new QWidget();
  screenShotLabel->resize(200,200);
  screenShotLabel->show();
  display(screenShotLabel);
  */
  
  
  recordButton = new QPushButton(tr("Record"));
  stopButton = new QPushButton(tr("Stop Recording"));
  stopButton->hide();
  selectButton = new QPushButton(tr("Select Window"));
  locationButton = new QPushButton(tr("Location"));
 
  
  address = new QLineEdit;
  address->setReadOnly(true);
  address->setText(path[0]);

  connect(recordButton, SIGNAL(clicked(bool)), this, SLOT(startRecording()));
  connect(stopButton, SIGNAL(clicked(bool)), this, SLOT(stopRecording()));
  connect(locationButton, SIGNAL(clicked(bool)), this, SLOT(location()));  
  
  
  QVBoxLayout *buttonsLayout= new QVBoxLayout;
  buttonsLayout->addWidget(recordButton);
  buttonsLayout->addWidget(stopButton);
  buttonsLayout->addWidget(selectButton);
  buttonsLayout->addWidget(locationButton);
  buttonsLayout->addStretch();
  
  QVBoxLayout *textLayout = new QVBoxLayout;
  textLayout->addWidget(address);
  
  QGridLayout *mainLayout = new QGridLayout(this);
  
  mainLayout->addLayout(buttonsLayout,0,0);  
  //mainLayout->addLayout(textLayout,1,0);
  //mainLayout->addWidget(screenShotLabel,0,1);
  
  setLayout(mainLayout);
  
  
  
  
}


void KUI_project::location()
{
    fileDiag = new QFileDialog;
    
    fileDiag->setFileMode(QFileDialog::DirectoryOnly);
    fileDiag->setDirectory(path[0]);
    
    connect(fileDiag, SIGNAL(currentChanged(QString)), this, SLOT(pathChenged()));
    
    
    fileDiag->show();

   
}


void KUI_project::pathChenged()
{
  
   path = fileDiag->selectedFiles();
   address -> setText(path[0]);
    
}

void KUI_project::startRecording()
{
  recordButton->hide();
  stopButton->show();
  
  char *sta = strdup( path[0].toAscii().data() ); 
  char location[strlen(sta)+10];
  memset(location, 0, (strlen(sta)+10));
  strcpy(location,sta);
  strcat(location,"video.avi");
  record(location);
  
}

void KUI_project::stopRecording()
{
  stopButton->hide();
  recordButton->show();
  
  stopRec();

}



#include "KUI_project.moc"
