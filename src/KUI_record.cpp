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

#include <gst/gst.h>
#include <iostream>
#include <QString>
#include <QRect>
#include <QDebug>
#include <cstring>

#include "KUI_record.h"


KUIRecord::KUIRecord(QString format, QRect area, QString camera, QString screen )
{

  GstElement *camSource, *camQueue1, *camColor, *camRate, *camMux, *camQueue2, *camSink;

  camSource = gst_element_factory_make ("v4l2src", "camSource");
  camColor = gst_element_factory_make ("ffmpegcolorspace", "camColorpsace");
  camRate = gst_element_factory_make("videorate", "camRate");
  camSink = gst_element_factory_make ("filesink", "camSink");
  camQueue1 = gst_element_factory_make("multiqueue", "queue1");
  camQueue2 = gst_element_factory_make("multiqueue", "queue2");
  
  if ( format==QString("avi") ) {
    camEnc = gst_element_factory_make ("xvidenc", "camEnc");
    camMux = gst_element_factory_make ("avimux", "camMux");
  } else {
    camEnc = gst_element_factory_make ("thoraenc", "camEnc");
    camMux = gst_element_factory_make ("oggmux", "camMux");
  }
  
  GstElement *screenSource, *screenRate, *screenColor, *screenMux, *screenSink; 
  
  screenSource = gst_element_factory_make("ximagesrc", "screenSource");
  screenRate = gst_element_factory_make("videorate", "screenRate");
  screenColor = gst_element_factory_make("ffmpegcolorspace", "screenColor");
  screenSink = gst_element_factory_make("filesink", "screenSink");
  
  if ( format==QString("avi") ) {
    screenEnc = gst_element_factory_make("xvidenc", "screenEnc");
    screenMux = gst_element_factory_make("avimux", "screenMux");
  } else {
    screenEnc = gst_element_factory_make("theoraenc", "screenEnc");
    screenMux = gst_element_factory_make("oggmux", "screenMux");
  }

  pipeline = gst_pipeline_new ("recorder");
  

  
  qDebug() << camera;
  qDebug() << screen;
  
  char cameraChar[ camera.length()+1 ];
  memset( cameraChar, 0, camera.length()+1 );
  
  char screenChar[ screen.length()+1 ];
  memset( screenChar, 0, screen.length()+1 );
  
  for ( int i=0; i!=camera.length(); ++i ) {
    cameraChar[i] = camera.at(i).toAscii();
  }
  
  for ( int i=0; i!=screen.length(); ++i ) {
    screenChar[i] = screen.at(i).toAscii();
  }
  
  //g_object_set (G_OBJECT (screenSource), "use-damage", true , NULL);
  g_object_set (G_OBJECT (screenSource), "startx", area.topLeft().x() , NULL);
  g_object_set (G_OBJECT (screenSource), "starty", area.topLeft().y() , NULL);
  g_object_set (G_OBJECT (screenSource), "endx", area.bottomRight().x() , NULL);
  g_object_set (G_OBJECT (screenSource), "endy", area.bottomRight().y() , NULL);
  g_object_set (G_OBJECT (camSink), "location", cameraChar,  NULL);
  g_object_set (G_OBJECT (screenSink), "location", screenChar,  NULL);
  
  GstCaps *caps;

  caps = gst_caps_new_simple ("video/x-raw-rgb",
			      "framerate", GST_TYPE_FRACTION, 20, 1,
			      // "width", G_TYPE_INT , 200,
	  	              // "height", G_TYPE_INT, 200,
	  	             // "pixel-aspect-ratio", GST_TYPE_FRACTION, 1 , 1,
		             NULL);
  
  gst_bin_add_many (GST_BIN (pipeline),
			     screenSource,
			     screenRate,
			     screenColor,
			     screenEnc,
			     screenMux,
			     screenSink,
			     camSource,
			     camColor,
			     camQueue1,
			     camRate,
			     camEnc,
			     camMux,
			     camQueue2,
			     camSink,
			     NULL);
  
  gst_element_link_filtered (screenSource, screenColor, caps);
  gst_element_link_many (screenColor, screenRate, screenEnc, screenMux, screenSink, NULL);
  gst_element_link_many (camSource, camColor, camRate, camEnc, camMux, camSink, NULL);
  
  gst_element_set_state (pipeline, GST_STATE_PLAYING);

 
}

KUIRecord::~KUIRecord()
{
  gst_element_send_event (screenEnc, gst_event_new_eos ());
  gst_element_send_event (camEnc, gst_event_new_eos ());
  gst_element_send_event(pipeline, gst_event_new_flush_stop());
  gst_element_set_state(pipeline, GST_STATE_NULL);
  gst_object_unref(pipeline);  
}
