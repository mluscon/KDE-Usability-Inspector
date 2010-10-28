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
#ifndef KUI_CAMERAWIDGET_H
#define KUI_CAMERAWIDGET_H

#include <QWidget>
#include <gst/gst.h>

class cameraWidget : public QWidget 
{
  Q_OBJECT
  
  public:
    cameraWidget(QWidget *parent);
    virtual ~cameraWidget();
 
  private:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);
    GstElement *cameraPipeline;
    GstElement *sink;
    
  public:
    void stopPipeline();
   
};

#endif