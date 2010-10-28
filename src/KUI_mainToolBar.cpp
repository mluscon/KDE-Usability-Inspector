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

#include "KUI_mainToolBar.h"
#include "KUI_record.h"

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


mainToolBar::mainToolBar(QWidget* parent): KToolBar(parent)
{
  startx = starty = endx = endy = 0;
  
  trayIcon = new KSystemTrayIcon("media-playback-stop",0);
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

void mainToolBar::updateInterface(Mode mode)
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




void mainToolBar::aimSlot()
{
  Display *dpy = XOpenDisplay(NULL);
  
  int status;
  Cursor cursor;
  XEvent event;
  Window target_win = None, root = RootWindow(dpy,NULL);
  int buttons = 0;

  /* Make the target cursor */
  cursor = XCreateFontCursor(dpy, XC_crosshair);

  /* Grab the pointer using target cursor, letting it room all over */
  status = XGrabPointer(dpy, root, False,
			ButtonPressMask|ButtonReleaseMask, GrabModeSync,
			GrabModeAsync, root, cursor, CurrentTime);
  

  /* Let the user select a window... */
  while ((target_win == None) || (buttons != 0)) {
    /* allow one more event */
    XAllowEvents(dpy, SyncPointer, CurrentTime);
    XWindowEvent(dpy, root, ButtonPressMask|ButtonReleaseMask, &event);
    switch (event.type) {
    case ButtonPress:
      if (target_win == None) {
	target_win = event.xbutton.subwindow; /* window selected */
	if (target_win == None) target_win = root;
      }
      buttons++;
      break;
    case ButtonRelease:
      if (buttons > 0) /* there may have been some down before we started */
	buttons--;
       break;
    }
  } 

  XUngrabPointer(dpy, CurrentTime);      /* Done with pointer */


  
  XWindowAttributes selWin;
  XGetWindowAttributes(dpy, target_win, &selWin);
 
  startx = selWin.x;
  starty = selWin.y;
  endx = selWin.x + selWin.width;
  endy = selWin.y + selWin.height;
  
}


void mainToolBar::pauseSlot()
{

}

void mainToolBar::playSlot()
{

}

void mainToolBar::recordSlot()
{
  trayIcon->show();
  this->parentWidget()->setVisible(false);
}

void mainToolBar::stopSlot()
{

}

void mainToolBar::lockSlot()
{

}

void mainToolBar::unhideSlot()
{
  this->parentWidget()->setVisible(true);
  trayIcon->setVisible(false);
}
