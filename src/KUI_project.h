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
 *  You should have received a copy of the GNU General Public License along with        *
 * this program.  If not, see <http://www.gnu.org/licenses/>.                           *
****************************************************************************************/

#ifndef KUI_PROJECT_H
#define KUI_PROJECT_H

#include "model/dommodel.h"
#include "KUI_CameraWidget.h"
#include "KUI_ScreenShotLabel.h"
#include "KUI_record.h"


#include <KMainWindow>
#include <QItemSelection>
#include <Phonon/VideoPlayer>
#include <Phonon/AudioOutput>
#include <Phonon/MediaSource>

class KActionCollection;
class KUrl;
class KSystemTrayIcon;
class KConfig;
class QListView;
class MainToolBar;
class QRect;
class QHBoxLayout;
class QTimer;

class KUI_project : public KMainWindow
{
  Q_OBJECT
  
public:
  KUI_project(QWidget *parent=0);
  ~KUI_project();
  void setProject();
  
  KActionCollection *collection;
  
private:
  void setupConfig();
  void setupMenuFile();
  void setupMenuWindow();
  void setupMenuSettings();
  void setupCentralWidget();
  void setupActions();
  
  KMenuBar *menuBar; 
  
  KUrl *standartUrl;
  KConfig *config;
  
  DomModel *model;
  
  QHBoxLayout *mediaLayout;
  CameraWidget *camera;
  ScreenShotLabel *screen;
  Phonon::VideoPlayer *screenVideo;
  Phonon::VideoPlayer *cameraVideo;
  Phonon::AudioOutput *cameraAudio;
  QTimer *timer;
  
  QListView *usersList;
  MainToolBar *playBar;
  
  QRect recArea;
  
  KSystemTrayIcon *trayIcon;
  
  KUIRecord *recorder;
  
  QString path;
    
private slots:
  void newProjectDialogSlot();
  void userEditationSlot( QString );
  void modelSetup( QString );
  void saveFileSlot();
  void openFileSlot();
  void saveAsFileSlot();
  void updateActions( QItemSelection selected, QItemSelection deselected );
  
  void aimSlot();
  void playSlot();
  void recordSlot();
  void pauseSlot();
  void stopSlot();
  void unhideSlot();
  void seekSlot( int );

signals:
  void playVideo();
    
};
    
    
    
#endif