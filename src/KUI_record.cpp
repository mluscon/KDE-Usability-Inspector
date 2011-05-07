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
  camQueue1 = gst_element_factory_make("multiqueue", "camqueue1");
  camQueue2 = gst_element_factory_make("multiqueue", "camqueue2");
  
  if ( format==QString("avi") ) {
    camEnc = gst_element_factory_make ("xvidenc", "camEnc");
    camMux = gst_element_factory_make ("avimux", "camMux");
  } else {
    camEnc = gst_element_factory_make ("theoraenc", "camEnc");
    camMux = gst_element_factory_make ("oggmux", "camMux");
  }
  
  GstElement *screenSource, *screenRate, *screenColor, *screenMux, *screenSink, *screenQueue1, *screenQueue2;
  
  screenSource = gst_element_factory_make("ximagesrc", "screenSource");
  screenRate = gst_element_factory_make("videorate", "screenRate");
  screenColor = gst_element_factory_make("ffmpegcolorspace", "screenColor");
  screenSink = gst_element_factory_make("filesink", "screenSink");
  screenQueue1 = gst_element_factory_make("multiqueue", "screenqueue1");
  screenQueue2 = gst_element_factory_make("multiqueue", "screenqueue2");
  
  
  if ( format==QString("avi") ) {
    screenEnc = gst_element_factory_make("xvidenc", "screenEnc");
    screenMux = gst_element_factory_make("avimux", "screenMux");
  } else {
    screenEnc = gst_element_factory_make("theoraenc", "screenEnc");
    screenMux = gst_element_factory_make("oggmux", "screenMux");
  }

  pipeline = gst_pipeline_new ("recorder");
  
  camera.append( "." ).append( format );
  screen.append( "." ).append( format );
  

  
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
  
  g_object_set (G_OBJECT (screenSource), "use-damage", false , NULL);
  g_object_set (G_OBJECT (screenSource), "startx", area.topLeft().x() , NULL);
  g_object_set (G_OBJECT (screenSource), "starty", area.topLeft().y() , NULL);
  g_object_set (G_OBJECT (screenSource), "endx", area.bottomRight().x() , NULL);
  g_object_set (G_OBJECT (screenSource), "endy", area.bottomRight().y() , NULL);
  
  g_object_set (G_OBJECT (camMux), "max-delay", 5000 , NULL);
  
  
  g_object_set (G_OBJECT (screenQueue1), "max-size-buffers", 10 , NULL);
  g_object_set (G_OBJECT (screenQueue2), "max-size-buffers", 10 , NULL);
  g_object_set (G_OBJECT (camQueue1), "max-size-buffers", 10 , NULL);
  g_object_set (G_OBJECT (camQueue1), "max-size-buffers", 10 , NULL);
  
  g_object_set (G_OBJECT (camSink), "location", cameraChar,  NULL);
  g_object_set (G_OBJECT (screenSink), "location", screenChar,  NULL);
  
  GstCaps *screenCaps;

  screenCaps = gst_caps_new_simple ("video/x-raw-rgb",
			      "framerate", GST_TYPE_FRACTION, 10, 1,
			      // "width", G_TYPE_INT , 200,
	  	              // "height", G_TYPE_INT, 200,
	  	              //"pixel-aspect-ratio", GST_TYPE_FRACTION, 1 , 1,
		             NULL);
  
  GstCaps *cameraCaps;

  cameraCaps = gst_caps_new_simple ("video/x-raw-rgb",
			      "framerate", GST_TYPE_FRACTION, 10, 1,
			      // "width", G_TYPE_INT , 200,
	  	              // "height", G_TYPE_INT, 200,
	  	              //"pixel-aspect-ratio", GST_TYPE_FRACTION, 1 , 1,
		             NULL);
  
    
  
  GstElement *audioSource, *audioRate, *audioConvert, *audioSample;
  
  audioSource = gst_element_factory_make("autoaudiosrc", "audioSource");
  audioRate = gst_element_factory_make("audiorate", "audioRate");
  audioConvert = gst_element_factory_make("audioconvert", "audioConvert");
  audioSample = gst_element_factory_make("audiosample", "audioSample" );
  audioEnc = gst_element_factory_make("vorbisenc", "audioEnc");
  
  
  gst_bin_add_many (GST_BIN (pipeline),
			     screenSource,
			     screenRate,
			     screenColor,
			     screenEnc,
			     screenMux,
		             screenSink,
			     screenQueue1,
			     screenQueue2,
		    
		             camSource,
			     camColor,
			     camQueue1,
			     camRate,
			     camEnc,
			     camMux,
			     camQueue2,
			     camSink,
		
			     audioSource,
			     
			     audioConvert,
			     audioRate,
			     audioEnc,
		  
			     NULL);
  
  
  
  gst_element_link_filtered (screenSource, screenColor, screenCaps);
  gst_element_link_many (screenColor, screenRate, screenQueue1, screenEnc,screenQueue2 , screenMux, screenSink, NULL);

  
  gst_element_link_filtered (camSource, camColor, cameraCaps);
  gst_element_link_many ( camColor, camRate, camQueue2, camEnc, camQueue1, camMux, camSink,  NULL);
  
  gst_element_link_many ( audioSource, audioConvert, audioRate, audioEnc, camQueue1, camMux,  NULL);
  
  gst_element_set_state (pipeline, GST_STATE_PLAYING);

}

KUIRecord::~KUIRecord()
{
  gst_element_send_event (screenEnc, gst_event_new_eos ());
  gst_element_send_event (camEnc, gst_event_new_eos ());
  gst_element_send_event (audioEnc, gst_event_new_eos());
  gst_element_send_event(pipeline, gst_event_new_flush_stop());
  gst_element_set_state(pipeline, GST_STATE_NULL);
  gst_object_unref(pipeline);  
}

