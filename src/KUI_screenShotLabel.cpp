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

#include "KUI_screenShotLabel.h"

#include <QApplication>
#include <QPixmap>
#include <QDesktopWidget>
#include <QTimer>
#include <QPainter>
#include <X11/Xlib.h>  


screenShotLabel::screenShotLabel(QWidget* parent): QLabel(parent)
{
  this->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
  
  timer = new QTimer(this);
  timer->start(1000);
  connect(timer, SIGNAL(timeout()), this, SLOT(pictureUpdate()));
}

void screenShotLabel::resizeEvent(QResizeEvent* event)
{
    pictureUpdate();
    QWidget::resizeEvent(event);
}



void screenShotLabel::pictureUpdate()
{
  QPixmap grabedPixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
  

  screenPixmap = grabedPixmap.scaled(this->width()-5,this->height()-5, Qt::KeepAspectRatio, Qt::SmoothTransformation); 
  this->setPixmap(screenPixmap);
  
}

