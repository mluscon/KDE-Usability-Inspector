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
#include <QX11Info>


#include "KUI_project.h"
#include "KUI_record.h"
#include "KUI_select.h"

KUI_project::KUI_project(QWidget* parent): QWidget(parent)
{
   
  avi=true;
  setWindowTitle(tr("KDE Usability Inspector"));
  resize(360,240);
  path.push_back(QDir::homePath());
  path[0].push_back("/");
  
  fps=10;
  sX=0;
  sY=0;
  eX=0;
  eY=0;  
  
  screenShotLabel = new QWidget();
  screenShotLabel->setAttribute(Qt::WA_NativeWindow);
 
  recordButton = new QPushButton(tr("Record"));
  recordButton->setIcon(QIcon("../media-record.svg"));
  
  stopButton = new QPushButton(tr("Stop Recording"));
  stopButton->setIcon(QIcon("../media-playback-stop.svg"));
  stopButton->hide();
  
  selectButton = new QPushButton(tr("Select Window"));
 
  locationButton = new QPushButton(tr("Save As"));
  locationButton->setIcon(QIcon(":/trolltech/styles/commonstyle/images/standardbutton-save-128.png"));
  
  aviButton = new QRadioButton("avi");
  aviButton->setChecked(true);
  
  oggButton = new QRadioButton("ogg");
  
  fpsChanger = new QSpinBox();
  fpsChanger->setRange(1,25);
  fpsChanger->setValue(fps);
  
  fpsLabel = new QLabel(tr("fps"));
   
  
  
  QHBoxLayout *settingsLayout = new QHBoxLayout;
  settingsLayout->addWidget(aviButton);
  settingsLayout->addWidget(oggButton);
  settingsLayout->addWidget(fpsChanger);
  settingsLayout->addWidget(fpsLabel); 
  
  
   
  
  address = new QLineEdit;
  address->setReadOnly(true);
  address->setText(path[0]);

  connect(recordButton, SIGNAL(clicked(bool)), this, SLOT(startRecording()));
  connect(stopButton, SIGNAL(clicked(bool)), this, SLOT(stopRecording()));
  connect(selectButton, SIGNAL(clicked(bool)), this, SLOT(winSelect()));
  connect(locationButton, SIGNAL(clicked(bool)), this, SLOT(location()));  
  connect(aviButton, SIGNAL(clicked(bool)), this, SLOT(aviChecked()));
  connect(oggButton, SIGNAL(clicked(bool)), this, SLOT(oggChecked()));
  connect(fpsChanger, SIGNAL(valueChanged(int)), this, SLOT(fpsChanged()));
  
  
  QVBoxLayout *buttonsLayout= new QVBoxLayout;
  buttonsLayout->addWidget(recordButton);
  buttonsLayout->addWidget(stopButton);
  buttonsLayout->addWidget(selectButton);
  buttonsLayout->addWidget(locationButton);
  buttonsLayout->addLayout(settingsLayout);
  buttonsLayout->addStretch();
 
  
  QVBoxLayout *textLayout = new QVBoxLayout;
  textLayout->addWidget(address);
  
  QGridLayout *mainLayout = new QGridLayout(this);
  
  mainLayout->addLayout(buttonsLayout,0,0);  
  //mainLayout->addLayout(textLayout,1,0);
  mainLayout->addWidget(screenShotLabel,0,1);
  
  setLayout(mainLayout);
  
  QApplication::syncX();
 
}


KUI_project::~KUI_project()
{
  endGst();
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

void KUI_project::startRecording( )
{
  pausedDisplay();
  recordButton->hide();
  stopButton->show();
  
  
  if (avi) { 
    recordAvi(&path[0], fps, sX, sY, eX, eY); 
  } else { 
    recordOgg(&path[0], fps, sX, sY, eX, eY);
  }
  
  
}

void KUI_project::stopRecording()
{
  
  stopRec();
  stopButton->hide();
  recordButton->show();
  unpausedDisplay();

}

void KUI_project::winSelect()
{
  selectWindow(&sX, &sY, &eX, &eY);
}




void KUI_project::aviChecked()
{
  aviButton->setChecked(true);
  avi=true;
}

void KUI_project::oggChecked()
{
  oggButton->setChecked(true);
  avi=false;
}

void KUI_project::fpsChanged()
{
  fps=fpsChanger->value();
}


#include "KUI_project.moc"
