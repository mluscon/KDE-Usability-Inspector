/****************************************************************************************
 * Copyright (c) 2010 Michal Luscon <mluscon@gmail.com>                                 *
 *                                                                                      *
 * This program is free software you can redistribute it and/or modify it under        *
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

#include "KUI_NewProjectDialog.h"
#include "dommodel.h"

#include <QListView>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QDataWidgetMapper>
#include <KLineEdit>
#include <QSpinBox>
#include <KLocale>
#include <QLabel>
#include <QToolButton>
#include <KFileDialog>
#include <QDir>
#include <KMessageBox>
#include <KFile>
#include <QDebug>
#include <QDomDocument>
#include <QTreeView>


NewProjectDialog::NewProjectDialog(QWidget* parent): KDialog(parent)
{
  this->setWindowTitle(i18n("Create New Project"));
  sessionFolder = QDir::homePath();
  sessionName = i18n("New Session");
  connect(this, SIGNAL(okClicked()), this, SLOT(okButtonSlot()));
  
  QWidget *centralWidget = new QWidget(this);
  this->setMainWidget( centralWidget );
  
  QVBoxLayout *centraLayout = new QVBoxLayout;
  centralWidget->setLayout( centraLayout );
  
  nameLine = new KLineEdit;
  nameLine->setText(i18n("New Session"));
  connect( nameLine, SIGNAL(textChanged(QString)), this, SLOT(nameChanged(QString)) );
  
  folderLine = new KLineEdit;
  folderLine->setText( sessionFolder );
  connect( folderLine, SIGNAL(textChanged(QString)), this, SLOT(folderChanged(QString)) );
  
  QToolButton *setFolder = new QToolButton;
  setFolder->setIcon(KIcon("document-open-folder"));
  connect(setFolder, SIGNAL(clicked(bool)), this, SLOT(setSessionFolder()) );
  
  QGridLayout *project = new QGridLayout;
  project->addWidget(new QLabel(i18n("Session name:"), 0, 0));
  project->addWidget(nameLine, 0, 1);
  project->addWidget(new QLabel(i18n("Session folder:")), 1, 0);
  project->addWidget( folderLine, 1, 1 );
  project->addWidget( setFolder, 1, 2);
  
  centraLayout->addLayout( project );

}

void NewProjectDialog::setSessionFolder()
{
   QString folder = KFileDialog::getExistingDirectory( KUrl( sessionFolder ), this, QString(i18n("Set Directory")) );
   
   if ( folder.isEmpty() ) return;
   
   folderLine->setText(folder);
   sessionFolder=folder;   
   
}

void NewProjectDialog::folderChanged(QString newFolder)
{
  sessionFolder = newFolder;
}

void NewProjectDialog::nameChanged(QString newName)
{
  sessionName = newName;
}

void NewProjectDialog::okButtonSlot()
{
  QDir directory( sessionFolder );
  if ( !directory.exists() ) {
    KMessageBox::error( this, i18n("Session folder does not exist."), i18n("Error"));
    return;
   }

  if ( sessionFolder[ sessionFolder.length()-1 ] != '/' ) { sessionFolder.append('/'); }

  QString xuiFilePath;
  xuiFilePath.append(sessionFolder);
  xuiFilePath.append(sessionName);
  xuiFilePath.append( QString(".xml") );
  
  QFile newFile( xuiFilePath );
  
  if ( newFile.exists() ) {
    if ( !newFile.remove() ) {
      KMessageBox::error( this, i18n("Error1"), i18n("Error"));
      return;
    }
  }
    
    
  if ( !newFile.open(QIODevice::WriteOnly) ) {
    KMessageBox::error( this, i18n("Error1"), i18n("Error"));
    return;
  }
  
  QDomDocument document;
  
  QDomElement elem = document.createElement("session");
  elem.setAttribute( "name" , sessionName);
  document.appendChild( elem );
   
  QTextStream stream( &newFile );
  
  document.save( stream, 0 );
  
  newFile.close();
  
  UsersEditationDialog *dialog = new UsersEditationDialog(this, xuiFilePath );
  dialog->show();
  
}




UsersEditationDialog::UsersEditationDialog(QWidget* parent, QString xuiPath ): KDialog( parent )
{
  QWidget *central = new QWidget;
  QVBoxLayout *centralLayout = new QVBoxLayout;
  central->setLayout( centralLayout );
  this->setMainWidget( central );
  
  QGridLayout *buttonLayout = new QGridLayout;
  buttonLayout->addWidget( new QLabel( i18n("Add/Remove User" )), 0 ,0 );
  
  QToolButton *addUserButton = new QToolButton;
  addUserButton->setIcon( KIcon("list-add") );
  connect( addUserButton, SIGNAL(clicked(bool)), this, SLOT(addUser()) ); 
  buttonLayout->addWidget( addUserButton, 0, 1 );
  
  
  QToolButton *removeUserButton = new QToolButton;
  removeUserButton->setIcon( KIcon("list-remove") );
  connect( removeUserButton, SIGNAL(clicked(bool)), this, SLOT(removeUser()) );
  buttonLayout->addWidget( removeUserButton, 0, 2 );
  
  centralLayout->addLayout( buttonLayout );
  
  

  
  model = new DomModel( xuiPath, this);
  tree = new QTreeView;
  tree->setAllColumnsShowFocus(true);
  list = new QListView;
  
  tree->setModel( model );
  tree->setRootIndex( model->index(0, 0, QModelIndex() ));
  tree->setColumnHidden( 1, true);
  tree->setColumnHidden( 2, true);
  tree->setHeaderHidden( true );

  
  QHBoxLayout *viewLayout = new QHBoxLayout;
  viewLayout->addWidget( tree );

  
  centralLayout->addLayout( viewLayout );
  

}

#include <QModelIndex>
#include "domitem.h"
#include <QDebug>

void UsersEditationDialog::addUser()
{
  QModelIndex sessionIndex = model->index(0, 0,QModelIndex());
  DomItem *sessionItem = static_cast<DomItem*> ( sessionIndex.internalPointer() );
  qDebug() << sessionItem->node().nodeName();
  
  model->insertRows(0,1, sessionIndex);
    
  
  
}

void UsersEditationDialog::removeUser()
{
  QModelIndex current = tree->selectionModel()->currentIndex();
  model->removeRows( current.row(), 1, current.parent() );

}



