/***************************************************************************************
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

#include "KUI_project.h"
#include "KUI_MainToolBar.h"
#include "KUI_NewProjectDialog.h"

#include <KAction>
#include <KLocale>
#include <KStandardAction>
#include <KActionCollection>
#include <KMenuBar>
#include <KMenu>
#include <KApplication>
#include <KAboutApplicationDialog>
#include <QPushButton>
#include <KFileDialog>
#include <KUrl>
#include <QFileDialog>
#include <KConfig>
#include <KConfigDialog>
#include <QDomDocument>
#include <QTextStream>
#include <QIODevice>
#include <QDomNode>
#include <QFile>

KUI_project::KUI_project(QWidget* parent): KMainWindow(parent)
{
  setupConfig();
  
  this->resize(700,300);
    
  collection = new KActionCollection(this);
  defaultCentral = new KuiCentralWidget(this);
  
  menuBar = new KMenuBar;
  setupMenuFile();
  setupMenuWindow();
  setupMenuSettings();
  menuBar->addMenu(helpMenu());
  this->setMenuBar(menuBar);
  
  KToolBar *tools = new KToolBar(i18n("&Tools"), this);
  tools->addAction(collection->action("new_file"));
  tools->addAction(collection->action("open_file"));
  tools->addAction(collection->action("save_file"));
  tools->setToolButtonStyle(Qt::ToolButtonIconOnly);
  
  
  this->setCentralWidget(defaultCentral);
  
  MainToolBar *playBar = new MainToolBar(this);
  playBar->setAccessibleDescription("Play Bar");
  
  this->addToolBar(Qt::BottomToolBarArea, playBar);
  
}



void KUI_project::setupMenuFile()
{
  
  KMenu *fileMenu = new KMenu(i18n("&File"),this);
   
  KAction *action = KStandardAction::openNew(this, SLOT(newFileSlot()), collection);
  fileMenu->addAction(collection->addAction("new_file", action));
  
  fileMenu->addSeparator();
  
  action = KStandardAction::open(this, SLOT(openFileSlot()), collection);
  fileMenu->addAction(collection->addAction("open_file", action));
  
  action = KStandardAction::save(this, SLOT(saveFileSlot()), collection);
  action->setEnabled(false);
  fileMenu->addAction(collection->addAction("save_file", action)); 
  
  action = KStandardAction::saveAs(this, SLOT(saveAsFileSlot()), collection);
  action->setEnabled(false);      
  fileMenu->addAction(collection->addAction("save_as_file", action));
  
  fileMenu->addSeparator();
  
  action = KStandardAction::quit(this, SLOT(close()), collection);
  fileMenu->addAction(collection->addAction("quit", action)); 
  
  menuBar->addMenu(fileMenu);
}

void KUI_project::setupMenuWindow()
{
  KMenu *windowMenu = new KMenu(i18n("&Window"),this);
  
  KAction *showDesktop = new KAction(i18n("Show &Desktop"),this);
  showDesktop->setCheckable(true);
  showDesktop->setChecked(true);
  connect(showDesktop, SIGNAL(triggered(bool)), defaultCentral, SLOT(screenVis()));
  windowMenu->addAction(showDesktop);
  
  KAction *showCamera = new KAction(i18n("Show &Camera"),this);
  showCamera->setCheckable(true);
  showCamera->setChecked(true);
  windowMenu->addAction(showCamera);
  connect(showCamera, SIGNAL(triggered(bool)), defaultCentral, SLOT(cameraVis()));
  menuBar->addMenu(windowMenu);
}



void KUI_project::setupMenuSettings()
{
  KMenu *settingsMenu = new KMenu(i18n("&Settings"),this);
  
  menuBar->addMenu(settingsMenu);
  
  KAction *action = KStandardAction::configureToolbars(this, SLOT(), collection);
  settingsMenu->addAction(collection->addAction("configure_toolbars", action));
  
  action = KStandardAction::preferences(this, SLOT(), collection);
  settingsMenu->addAction(collection->addAction("configure_kui", action));
}

void KUI_project::preferencesSlot()
{
  //KConfigDialog *conf = new KConfigDialog(this, "KUI preferences", 


}


void KUI_project::setupConfig()
{
  KConfig newconf("KUI_project");
}




void KUI_project::newFileSlot()
{
  
  

  NewProjectDialog *dialogSettings = new NewProjectDialog(this);
    
  dialogSettings->setModal(true);
  dialogSettings->show();
  
  
}

void KUI_project::saveFileSlot()
{

}

void KUI_project::openFileSlot()
{
  KUrl url;
  url.setDirectory(QDir::homePath());
  QString path = KFileDialog::getOpenFileName(url, QString("*.xui"), this, QString("Open"));

  if (path.isEmpty()) {
    return;
  }
  
  QString projectFolder=path;
   
  
}

void KUI_project::saveAsFileSlot()
{


}


void KUI_project::setProject()
{
 
  
}


  