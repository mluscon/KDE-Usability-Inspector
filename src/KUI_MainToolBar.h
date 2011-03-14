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

#ifndef KUI_MAINTOOLBAR_H
#define KUI_MAINTOOLBAR_H

#include "KUI_record.h"

#include <KToolBar>


class KActionCollection;
class QSlider;
class KSystemTrayIcon;
class QAbstractItemModel;


enum Mode { Default, Capture, PlayStart, Playing, Pause };

class MainToolBar : public KToolBar
{
  Q_OBJECT
  
  public:
    MainToolBar(QWidget *parent);
  
  private:
    struct rect area;
    KActionCollection *toolBarCollection;
    QSlider *timeSlider;
    
    KSystemTrayIcon *trayIcon;
    KUIRecord *rec;
    QAbstractItemModel *model;
    
public:
  void updateInterface( Mode );
  void setModel( QAbstractItemModel );
 
    
  public slots:
    void aimSlot();
    void playSlot();
    void recordSlot();
    void pauseSlot();
    void stopSlot();
    void lockSlot();
    void unhideSlot();
};



#endif