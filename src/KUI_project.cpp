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
#include "model/dommodel.h"

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
#include <QDebug>
#include <QLayout>
#include <QHBoxLayout>
#include <QListView>
#include <QDebug>
#include <KSystemTrayIcon>
#include <QDockWidget>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <QTimer>



KUI_project::KUI_project(QWidget* parent): KMainWindow(parent)
{
  
  trayIcon = new KSystemTrayIcon("media-playback-stop", 0 );
  connect(trayIcon, SIGNAL( activated(QSystemTrayIcon::ActivationReason)), 
          this, SLOT( unhideSlot() ) );
  
  path.append( QDir::homePath() );
  
  
  recArea.setCoords( 0, 0, 0, 0);
  setupConfig();
  
  this->resize( 700, 300 );
    
  collection = new KActionCollection(this);
  
  setupCentralWidget();
  
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
 
  setupActions();
  
  playBar = new MainToolBar( collection, this);
  
  playBar->setAccessibleDescription("Play Bar");
  
  this->addToolBar(Qt::BottomToolBarArea, playBar);
  connect( playBar->timeSlider, SIGNAL( sliderMoved(int) ),
	   this, SLOT( seekSlot(int) ) );
  
  
}

KUI_project::~KUI_project()
{
  delete trayIcon;
  
  if ( !timer )
    delete timer; 
}


void KUI_project::setupMenuFile()
{
  
  KMenu *fileMenu = new KMenu(i18n("&File"),this);
   
  KAction *action = KStandardAction::openNew(this, SLOT(newProjectDialogSlot()), collection);
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
  //connect(showDesktop, SIGNAL(triggered(bool)), defaultCentral, SLOT(screenVis()));
  windowMenu->addAction(showDesktop);
  
  KAction *showCamera = new KAction(i18n("Show &Camera"),this);
  showCamera->setCheckable(true);
  showCamera->setChecked(true);
  windowMenu->addAction(showCamera);
  //connect(showCamera, SIGNAL(triggered(bool)), defaultCentral, SLOT(cameraVis()));
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

void KUI_project::setupConfig()
{
  KConfig newconf("KUI_project");
}


void KUI_project::setupCentralWidget()
{
  QWidget *central = new QWidget( this );
  QGridLayout *centralLayout = new QGridLayout( this );
  central->setLayout( centralLayout );
  
  camera = new CameraWidget( this );
  screen = new ScreenShotLabel( this );
  QWidget *mediaWidget = new QWidget( this );
  mediaWidget->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
  
  mediaLayout = new QHBoxLayout( this );
  mediaLayout->addWidget(screen);
  mediaLayout->addWidget(camera);
  mediaWidget->setLayout(mediaLayout);
  
  usersList = new QListView(this);
  usersList->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
  usersList->setMaximumWidth(100);

  
  centralLayout->addWidget(usersList,0,0);
  centralLayout->addWidget(mediaWidget, 0, 1);
    
  setCentralWidget( central );
  
  screenVideo = new Phonon::VideoPlayer( this );
  screenVideo->setVisible( false );
  mediaLayout->addWidget( screenVideo );
  
  cameraVideo = new Phonon::VideoPlayer( this );
  cameraVideo->setVisible( false );
  mediaLayout->addWidget( cameraVideo );
  
  cameraAudio = new Phonon::AudioOutput( this );

}

void KUI_project::setupActions()
{
  
  KAction *action = new KAction(this);
  
  action->setIcon(KIcon("edit-delete"));
  action->setText(i18n("&Aim"));
  collection->addAction("aim", action);
  connect(action, SIGNAL(triggered(bool)), this, SLOT(aimSlot()));
  
  
  action = new KAction(this);
  action->setIcon(KIcon("media-record"));
  action->setText(i18n("&Record"));
  connect(action, SIGNAL(triggered(bool)), this, SLOT(recordSlot()));
  collection->addAction("record", action);
  
  
  action = new KAction(this);
  action->setIcon(KIcon("media-playback-start"));
  action->setText(i18n("&Play"));
  connect(action, SIGNAL(triggered(Qt::MouseButtons,Qt::KeyboardModifiers)), this , SLOT(playSlot()));
  collection->addAction("play",action);

  action = new KAction(this);
  action->setIcon(KIcon("media-playback-stop"));
  action->setText(i18n("&Stop"));
  connect(action, SIGNAL(triggered(Qt::MouseButtons,Qt::KeyboardModifiers)), this , SLOT(stopSlot()));
  collection->addAction("stop",action);
 
  
}


void KUI_project::updateActions(QItemSelection selected, QItemSelection deselected)
{
  QModelIndex item = usersList->selectionModel()->currentIndex();
  
  item = model->index( item.row(), 1, item.parent());  
  QVariant screenPath = model->data( item, Qt::DisplayRole );
  
  item = model->index( item.row(), 2, item.parent());  
  QVariant cameraPath = model->data( item, Qt::DisplayRole );
    

  if ( screenPath.toString() == "empty" || cameraPath.toString() == "empty" ) {
    playBar->updateInterface( Capture );
    screenVideo->setVisible( false );
    cameraVideo->setVisible( false );
    screen->setVisible( true );
    camera->setVisible( true );
    return;
  }
  
  if ( screenPath.toString() != "empty" && cameraPath.toString() != "empty" ) {
    Phonon::MediaSource screenSource( screenPath.toString() );
    Phonon::MediaSource cameraSource( cameraPath.toString() );
    
    cameraVideo->load( cameraSource );
    cameraVideo->setMinimumSize( 200, 200);
      
    screenVideo->load( screenSource );
      
    playBar->updateInterface( PlayStart );
    
  
    screen->setVisible( false );
    camera->setVisible( false );    
    screenVideo->setVisible( true );
    cameraVideo->setVisible( true );
    return;
  }

}



void KUI_project::newProjectDialogSlot()
{
  
  NewProjectDialog *dialogSettings = new NewProjectDialog(this);
  connect( dialogSettings, SIGNAL( newProjectEnd(QString)),
           this, SLOT( userEditationSlot( QString )) );
  
  
  dialogSettings->setModal(true);
  dialogSettings->show();   
}

void KUI_project::userEditationSlot( QString path )
{
  UsersEditationDialog *dialog = new UsersEditationDialog( this, path );
  connect( dialog, SIGNAL( userEditationComplete(QString)),
           this, SLOT(modelSetup(QString)) );
  dialog->show();
}

void KUI_project::modelSetup(QString modelPath)
{
  qDebug() << "opening model in: " << path;
  
  if ( !model  )
    delete model;
  
  model = new DomModel( modelPath, this );
  
  KUrl url;
  url.setPath( modelPath );
  
  path = url.directory();
  QDir::setCurrent( path );
  
  qDebug() << "changing current directory: " << path; 
  
  usersList->setModel( model );
  usersList->setRootIndex( model->index( 0, 0, QModelIndex() ));
      
  connect( usersList->selectionModel(), SIGNAL( selectionChanged(QItemSelection,QItemSelection)),
           this, SLOT( updateActions(QItemSelection,QItemSelection)) );
  
  playBar->updateInterface( Default );
  
}





void KUI_project::saveFileSlot()
{

}

void KUI_project::openFileSlot()
{
  
  KUrl url;
  url.setDirectory( path );
  QString modelPath = KFileDialog::getOpenFileName(url, QString("*.xui"), this, QString("Open"));
  
  if ( modelPath.isEmpty() )
    return;
  
  url.setPath( modelPath );
  
  path = url.directory();
  QDir::setCurrent( path );
  
  qDebug() << "New path: " << path;
  
  if (modelPath.isEmpty()) {
    return;
  }
  

  modelSetup( modelPath );
  
}

void KUI_project::saveAsFileSlot()
{


}


void KUI_project::setProject()
{
 
  
}

void KUI_project::aimSlot()
{
  Display *dpy = XOpenDisplay(NULL);
  
  int status;
  Cursor cursor;
  XEvent event;
  Window target_win = None, root = RootWindow(dpy,NULL);
  int buttons = 0;

  cursor = XCreateFontCursor(dpy, XC_crosshair);

  status = XGrabPointer(dpy, root, False,
                        ButtonPressMask|ButtonReleaseMask, GrabModeSync,
                        GrabModeAsync, root, cursor, CurrentTime);
  
  while ((target_win == None) || (buttons != 0)) {
   
    XAllowEvents(dpy, SyncPointer, CurrentTime);
    XWindowEvent(dpy, root, ButtonPressMask|ButtonReleaseMask, &event);
    switch (event.type) {
    case ButtonPress:
      if (target_win == None) {
        target_win = event.xbutton.subwindow; 
        if (target_win == None) target_win = root;
      }
      buttons++;
      break;
    case ButtonRelease:
      if (buttons > 0)
        buttons--;
       break;
    }
  } 

  XUngrabPointer(dpy, CurrentTime);    

  XWindowAttributes selWin;
  XGetWindowAttributes(dpy, target_win, &selWin);
  
  
  recArea.setTopLeft( QPoint( selWin.x, selWin.y ) );
  recArea.setBottomRight( QPoint( selWin.x+selWin.width, selWin.y+selWin.height ) );

  XCloseDisplay(dpy);
}

void KUI_project::recordSlot()
{
  trayIcon->show();
  this->setVisible(false);
  
  QModelIndex item = usersList->selectionModel()->currentIndex();
  item = model->index( item.row(), 0, item.parent());  
  QString name = model->data( item, Qt::DisplayRole ).toString();
    
  QString path;
  path.append( "./" ).append( name );
  
  QString camera = path;
  QString screen = path;
  
  camera.append( "_camera" );
  screen.append( "_screen" );
  
  recorder = new KUIRecord(QString("avi"), recArea, camera, screen );
  
  camera.append( ".avi" );
  screen.append( ".avi" );
  
  model->setData( model->index( item.row(), 1, item.parent() ), camera, Qt::EditRole );
  model->setData( model->index( item.row(), 2, item.parent() ), screen, Qt::EditRole );
  
}

void KUI_project::unhideSlot()
{
  delete recorder;
  trayIcon->setVisible(false);
  
  updateActions( QItemSelection(), QItemSelection() );
  
  this->setVisible(true);
  
  
  
}

void KUI_project::pauseSlot()
{

}

void KUI_project::playSlot()
{

  connect( this, SIGNAL( playVideo() ),  cameraVideo, SLOT( play() ) );
  connect( this, SIGNAL( playVideo() ), screenVideo, SLOT( play() ) );
  
  connect( cameraVideo, SIGNAL( finished() ), this, SLOT( stopSlot() ) );
  connect( screenVideo, SIGNAL( finished() ), this, SLOT( stopSlot() ) );
  
  playBar->updateInterface( Playing );
  
  timer = new QTimer();
  connect( timer, SIGNAL( timeout() ), playBar, SLOT( timeShift() ) );
  timer->start( 1000 );
     
  emit playVideo();
  
  qDebug() << "Video length: " << cameraVideo->totalTime()/1000;
  playBar->timeSlider->setMaximum( cameraVideo->totalTime() / 1000 );
  
}

void KUI_project::stopSlot()
{
  timer->stop();
    
  cameraVideo->stop();
  screenVideo->stop();
  
  playBar->updateInterface( PlayStart );
}

void KUI_project::seekSlot( int seek )
{
  cameraVideo->seek( seek*1000 );
  screenVideo->seek( seek*1000 );


}



  