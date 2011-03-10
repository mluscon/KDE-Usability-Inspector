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
#include "./model/dommodel.h"
#include "./model/domitem.h"

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
#include <QModelIndex>
#include <KLineEdit>


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
    KMessageBox::error( 0, i18n("Session folder does not exist."), i18n("Error"));
    return;
   }

  if ( sessionFolder[ sessionFolder.length()-1 ] != '/' ) { sessionFolder.append('/'); }

  QString xuiFilePath;
  xuiFilePath.append(sessionFolder);
  xuiFilePath.append(sessionName);
  xuiFilePath.append( QString(".xui") );
  
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
  
  emit newProjectEnd( xuiFilePath );  
}




UsersEditationDialog::UsersEditationDialog(QWidget* parent, QString xuiPath ): KDialog( parent )
{
  path = xuiPath;
  QWidget *central = new QWidget;
  QGridLayout *centralLayout = new QGridLayout;
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
  
  centralLayout->addLayout( buttonLayout, 0, 0 );
  
  
  model = new DomModel( xuiPath, this);
  
  list = new QTreeView;    
  list->setModel( model );
  list->setRootIndex( model->index( 0, 0, QModelIndex()) );
  list->setMaximumWidth( 150 );
  list->setColumnHidden( 1, true );
  list->setColumnHidden( 2, true );
  
  connect( list->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
           this, SLOT( updateMappers(QItemSelection,QItemSelection)) );
   
  
  
  QHBoxLayout *viewLayout = new QHBoxLayout;
  viewLayout->addWidget( list );
  
    
  centralLayout->addLayout( viewLayout, 1, 0 );
  
  QGridLayout *mapperLayout = new QGridLayout;
  mapperLayout->addWidget( new QLabel(i18n("name:")), 0, 0 );
  
  mapper = new QDataWidgetMapper;
  mapper->setModel( model );
  mapper->setRootIndex( model->index( 0, 0, QModelIndex() ) );
  mapper->toFirst();
  mapper->setSubmitPolicy( QDataWidgetMapper::AutoSubmit );
  
  
  nameLine = new KLineEdit;
  mapper->addMapping( nameLine, 0 );
  mapperLayout->addWidget( nameLine, 0, 1 );
  
  centralLayout->addLayout( mapperLayout, 1, 1);
  
  connect( this, SIGNAL( okClicked() ),
           this, SLOT( okButtonSlot() ) );
  
}


void UsersEditationDialog::addUser()
{
  QModelIndex sessionIndex = model->index(0, 0,QModelIndex());
  DomItem *sessionItem = static_cast<DomItem*> ( sessionIndex.internalPointer() );
  qDebug() << sessionItem->node().nodeName();
  
  model->insertRows(0,1, sessionIndex);
    
  
  
}

void UsersEditationDialog::removeUser()
{
  QModelIndex current = list->selectionModel()->currentIndex();
  DomItem *item = static_cast<DomItem*>( current.internalPointer() );
  
  if ( current.isValid() && item->node().nodeName()=="user" )
    model->removeRows( current.row(), 1, current.parent() );

}


void UsersEditationDialog::updateMappers(QItemSelection first, QItemSelection last)
{
  mapper->setCurrentIndex( list->selectionModel()->currentIndex().row() );
  nameLine->setText( (model->data( list->selectionModel()->currentIndex(), 0 )).toString() );
  
  qDebug() << "current mapper index: " << mapper->currentIndex();
}

void UsersEditationDialog::okButtonSlot()
{
  qDebug() << "UsersEditationDialog okButtonSlot";
  emit userEditationComplete( path );
}






