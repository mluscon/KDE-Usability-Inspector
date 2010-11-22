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

#include "KUI_MainToolBar.h"


#include <KAction>
#include <KLocale>
#include <KStandardAction>
#include <KActionCollection>
#include <QSlider>
#include <QLayout>
#include <KSystemTrayIcon>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>


MainToolBar::MainToolBar(QWidget* parent): KToolBar(parent)
{
  area.startx = area.starty = area.endx = area.endy = 0;
  
  trayIcon = new KSystemTrayIcon("media-playback-stop",this);
  connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(unhideSlot()));
  
  this->setToolButtonStyle(Qt::ToolButtonIconOnly);
  
  toolBarCollection = new KActionCollection(this);
  
  KAction *action = new KAction(this);
  
  action->setIcon(KIcon("edit-delete"));
  action->setText(i18n("&Aim"));
  toolBarCollection->addAction("aim", action);
  connect(action, SIGNAL(triggered(bool)), this, SLOT(aimSlot()));
  this->addAction(action);
  
  
  action = new KAction(this);
  action->setIcon(KIcon("media-record"));
  action->setText(i18n("&Record"));
  connect(action, SIGNAL(triggered(bool)), this, SLOT(recordSlot()));
  toolBarCollection->addAction("record", action);
  this->addAction(action);
  
  
  action = new KAction(this);
  action->setIcon(KIcon("media-playback-start"));
  action->setText(i18n("&Play"));
  connect(action, SIGNAL(triggered(Qt::MouseButtons,Qt::KeyboardModifiers)), this , SLOT(playSlot()));
  toolBarCollection->addAction("play",action);
  this->addAction(action);
  
  
  action = new KAction(this);
  action->setIcon(KIcon("media-playback-stop"));
  action->setText(i18n("&Stop"));
  connect(action, SIGNAL(triggered(Qt::MouseButtons,Qt::KeyboardModifiers)), this , SLOT(stopSlot()));
  toolBarCollection->addAction("stop",action);
  this->addAction(action);
  
  this->addSeparator();
  
  
  timeSlider = new QSlider(this);
  timeSlider->setOrientation(Qt::Horizontal);
  this->addWidget(timeSlider);
  
  this->addSeparator();
    
  
  action = new KAction(this);
  action->setIcon(KIcon("kgpg"));
  action->setText(i18n("&Lock"));
  connect(action, SIGNAL(triggered(Qt::MouseButtons,Qt::KeyboardModifiers)), this , SLOT(lockSlot()));
  toolBarCollection->addAction("lock",action);
  this->addAction(action);
  
  updateInterface(defaultMode);
  
}

void MainToolBar::updateInterface(Mode mode)
{
  switch (mode) {
    case defaultMode:
     (toolBarCollection->action("aim"))->setEnabled(true);
     (toolBarCollection->action("play"))->setEnabled(false);
     (toolBarCollection->action("stop"))->setEnabled(false);
     timeSlider->setEnabled(false);
     (toolBarCollection->action("lock"))->setEnabled(false);
     break;
     
     case playingMode:
     (toolBarCollection->action("aim"))->setEnabled(false);
     (toolBarCollection->action("play"))->setEnabled(true);
     (toolBarCollection->action("stop"))->setEnabled(true);
     timeSlider->setEnabled(true);
     (toolBarCollection->action("lock"))->setEnabled(true);
     break;
     
     case stopMode:
     (toolBarCollection->action("aim"))->setEnabled(false);
     (toolBarCollection->action("play"))->setEnabled(true);
     (toolBarCollection->action("stop"))->setEnabled(false);
     timeSlider->setEnabled(false);
     (toolBarCollection->action("lock"))->setEnabled(true);
     break;
     
     case pauseMode:
     (toolBarCollection->action("aim"))->setEnabled(true);
     (toolBarCollection->action("play"))->setEnabled(true);
     (toolBarCollection->action("stop"))->setEnabled(true);
     timeSlider->setEnabled(true);
     (toolBarCollection->action("lock"))->setEnabled(true);
     break;
  }
    

}




void MainToolBar::aimSlot()
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
  
  
  area.startx = selWin.x;
  area.starty = selWin.y;
  area.endx = selWin.x + selWin.width;
  area.endy = selWin.y + selWin.height; 
  
  XCloseDisplay(dpy);
}


void MainToolBar::pauseSlot()
{

}

void MainToolBar::playSlot()
{

}

void MainToolBar::recordSlot()
{
  trayIcon->show();
  this->parentWidget()->setVisible(false);
  rec = new KUIRecord(QString("avi"), area, "/home/michal/"); 
}

void MainToolBar::stopSlot()
{
 
}

void MainToolBar::lockSlot()
{

}

void MainToolBar::unhideSlot()
{
  delete rec;
  this->parentWidget()->setVisible(true);
  trayIcon->setVisible(false);
}
