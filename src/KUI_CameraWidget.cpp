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

#include "KUI_CameraWidget.h"

#include <gst/interfaces/xoverlay.h>
#include <QApplication>
#include <iostream>
#include <KMessageBox>
#include <KLocale>

CameraWidget::CameraWidget(QWidget* parent): QWidget(parent)
{  
  cameraOK=true;
  this->setAttribute(Qt::WA_NativeWindow);
  
  GstElement *source, *color;
  
  cameraPipeline = gst_pipeline_new ("camera");
  source = gst_element_factory_make ("v4l2src", "source");
  color = gst_element_factory_make ("ffmpegcolorspace", "color");
  sink = gst_element_factory_make ("xvimagesink", "sink");

  
  if ( !cameraPipeline || !source || !color || !sink ) {
    if (cameraPipeline) { gst_object_unref(GST_OBJECT (cameraPipeline)); }
    if (source) { gst_object_unref(GST_OBJECT (source)); }
    if (color) { gst_object_unref(GST_OBJECT (color)); }
    if (sink) { gst_object_unref(GST_OBJECT (sink)); }
    cameraOK=false;
  }
  
  if (cameraOK) {
    gst_bin_add_many (GST_BIN (cameraPipeline), source, color, sink, NULL);
    
    GstCaps *caps;
    caps = gst_caps_new_simple ("video/x-raw-rgb",
      "framerate", GST_TYPE_FRACTION, 10, 1,
      //"width", G_TYPE_INT , 200,
      //"height", G_TYPE_INT, 200,
      "pixel-aspect-ratio", GST_TYPE_FRACTION, 1 , 1,
      NULL);
    
    gst_element_link_filtered (source, color, caps);
    gst_element_link_many (color, sink, NULL);
  } else {
    
  }
  
  
}

CameraWidget::~CameraWidget()
{
 
  if (cameraOK) { 
    gst_element_send_event (cameraPipeline, gst_event_new_eos ());
    gst_element_set_state(cameraPipeline, GST_STATE_NULL);
    gst_object_unref(cameraPipeline);
  }

}

void CameraWidget::showEvent(QShowEvent* event)
{
  QWidget::showEvent(event);
  
  if (cameraOK) { 
    QApplication::syncX();
    gst_element_set_state(sink, GST_STATE_READY);
    gst_element_set_state (cameraPipeline, GST_STATE_PLAYING);
    gst_x_overlay_prepare_xwindow_id(GST_X_OVERLAY(sink));
    gst_x_overlay_set_xwindow_id(GST_X_OVERLAY(sink),this->winId());
  } 
}

void CameraWidget::hideEvent(QHideEvent* event)
{
  QWidget::hideEvent(event);
  if (cameraOK) {
    gst_element_send_event (cameraPipeline, gst_event_new_eos ());
    gst_element_set_state(cameraPipeline, GST_STATE_NULL);
  }
}

void CameraWidget::showError(QString error)
{
  KMessageBox::messageBox(this, (KMessageBox::DialogType)8, error);
}

