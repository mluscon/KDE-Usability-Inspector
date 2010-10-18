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

#include <KAction>
#include <KLocale>
#include <KStandardAction>
#include <KActionCollection>
#include <QSlider>
#include <QLayout>

mainToolBar::mainToolBar(QWidget* parent): KToolBar(parent)
{
  this->setToolButtonStyle(Qt::ToolButtonIconOnly);
  
  KAction *recordAction = new KAction(this);
  
  recordAction->setIcon(KIcon("media-record"));
  recordAction->setText(i18n("&Record"));
  recordAction->isEnabled();
  
  connect(recordAction, SIGNAL(triggered(bool)), this, SLOT(playSlot));
  this->addAction(recordAction);
  
  KAction *playAction = new KAction(this);
  
  playAction->setIcon(KIcon("media-playback-start"));
  playAction->setText(i18n("&Play"));
  
  connect(playAction, SIGNAL(triggered(Qt::MouseButtons,Qt::KeyboardModifiers)), this , SLOT(playSlot()));
  this->addAction(playAction);
  
  KAction *stopAction = new KAction(this);
  
  stopAction->setIcon(KIcon("media-playback-stop"));
  stopAction->setText(i18n("&Stop"));
  
  connect(playAction, SIGNAL(triggered(Qt::MouseButtons,Qt::KeyboardModifiers)), this , SLOT(stopSlot()));
  this->addAction(stopAction);
  
  this->addSeparator();
  
  QSlider *timeSlider = new QSlider(this);
  timeSlider->setOrientation(Qt::Horizontal);
  this->addWidget(timeSlider);
  
  this->addSeparator();
  
  KAction *anotateAction = new KAction(this);
  
  anotateAction->setIcon(KIcon("kgpg"));
  anotateAction->setText(i18n("&Anotate"));
  
  connect(anotateAction, SIGNAL(triggered(Qt::MouseButtons,Qt::KeyboardModifiers)), this , SLOT(anotateSlot()));
  this->addAction(anotateAction);
  
}



void mainToolBar::pauseSlot()
{

}

void mainToolBar::playSlot()
{

}

void mainToolBar::recordSlot()
{

}

void mainToolBar::stopSlot()
{

}

void mainToolBar::anotateSlot()
{

}
