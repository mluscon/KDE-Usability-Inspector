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
#include <QSlider>
#include <QLayout>
#include <QTimer>
#include <KActionCollection>
#include <QDebug>



MainToolBar::MainToolBar(KActionCollection* collection, QWidget* parent): KToolBar( parent)
{
  setToolButtonStyle(Qt::ToolButtonIconOnly);
  
  toolBarCollection = collection;
  
  addAction( toolBarCollection->action("aim"));
  addAction( toolBarCollection->action("record"));
  addAction( toolBarCollection->action("play"));
  addAction( toolBarCollection->action("stop"));
  addSeparator();
    
  timeSlider = new QSlider(this);
  timeSlider->setOrientation(Qt::Horizontal);
  timeSlider->setMinimum( 0 );

  
  
  
  this->addWidget(timeSlider);
  
  this->addSeparator();
   
  updateInterface( Default );
  
}

void MainToolBar::updateInterface(Mode mode)
{
  switch (mode) {
    case Default:
     (toolBarCollection->action("aim"))->setEnabled( false );
     (toolBarCollection->action("record"))->setEnabled( false );
     (toolBarCollection->action("play"))->setEnabled( false );
     (toolBarCollection->action("stop"))->setEnabled( false );
     timeSlider->setEnabled( false );
     timeSlider->setValue( 0 );
     break;
     
     case Capture:
     (toolBarCollection->action("aim"))->setEnabled( true );
     (toolBarCollection->action("record"))->setEnabled( true );
     (toolBarCollection->action("play"))->setEnabled( false );
     (toolBarCollection->action("stop"))->setEnabled( false );
     timeSlider->setEnabled( false );
     timeSlider->setValue( 0 );
     break;
          
     case PlayStart:
     (toolBarCollection->action("aim"))->setEnabled( false );
     (toolBarCollection->action("record"))->setEnabled( false );
     (toolBarCollection->action("play"))->setEnabled( true );
     (toolBarCollection->action("stop"))->setEnabled( false );
     timeSlider->setEnabled( false );
     timeSlider->setValue( 0 );
     break;
     
     case Playing:
     (toolBarCollection->action("aim"))->setEnabled( false );
     (toolBarCollection->action("record"))->setEnabled( false );
     (toolBarCollection->action("play"))->setEnabled( false );
     (toolBarCollection->action("stop"))->setEnabled( true );
     timeSlider->setEnabled( true );
     break;
     
     case Pause:
     (toolBarCollection->action("aim"))->setEnabled( false );
     (toolBarCollection->action("record"))->setEnabled( false );
     (toolBarCollection->action("play"))->setEnabled( true );
     (toolBarCollection->action("stop"))->setEnabled( false );
     timeSlider->setEnabled( true );
     break;
  }
    

}

void MainToolBar::timeShift()
{
 
  timeSlider->setValue( (timeSlider->value() + 1 ) );

}
