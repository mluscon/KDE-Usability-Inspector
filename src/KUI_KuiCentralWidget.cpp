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

#include "KUI_KuiCentralWidget.h"
#include "KUI_screenShotLabel.h"


#include <Phonon/MediaObject>
#include <QGridLayout>

KuiCentralWidget::KuiCentralWidget(QWidget* parent): QWidget(parent)
{
  
  QGridLayout *centralLayout = new QGridLayout(this);
  
  Phonon::MediaObject *media = new Phonon::MediaObject(parent);
  vwidget = new Phonon::VideoWidget(parent);
  Phonon::createPath(media, vwidget);
   
  screen = new screenShotLabel(this);
   
  centralLayout->addWidget(screen,0,0);
  centralLayout->addWidget(vwidget,0,1);
  this->setLayout(centralLayout);
}


void KuiCentralWidget::camera(bool checked)
{
  if (checked) {
    screen->hide();
  } else {
    screen->show();
  }

}

void KuiCentralWidget::desktop(bool checked)
{

}






