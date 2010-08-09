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


#include <gst/gst.h>
#include <gst/interfaces/xoverlay.h>

#include <QApplication>
#include <QWidget>
#include <cstring>
#include <iostream>


GstElement *pipeline, *pipeline2, *enc;

int recordAvi(QString *path, int fps, unsigned int startX, unsigned int startY, unsigned int endX, unsigned int endY) {
	
	
	char *sta = strdup( path[0].toAscii().data() ); 
	char location[ strlen(sta)+10 ];
	memset(location, 0, ( strlen(sta)+10) );
	strcpy(location,sta);
	strcat(location,"video.avi");
	
	
	
	GstElement *source, *queue, *rate, *color, *queue2, *mux, *sink;
	
	pipeline = gst_pipeline_new ("desktop-recorder");
	if (!pipeline) { fputs("Pipeline error.",stderr); return 1; }

	source = gst_element_factory_make ("ximagesrc", "source");
	if (!source) { 
	  gst_object_unref(GST_OBJECT (pipeline));
	  return 1; }

	queue = gst_element_factory_make ("queue", "queue");
	if (!queue) { 
	  fputs("You need queue plugin installed.",stderr); 
	  gst_object_unref(GST_OBJECT (pipeline));
	  return 1; }

	rate = gst_element_factory_make ("videorate", "rate");
	if (!rate) { 
	  fputs("You need videorate plugin installed.",stderr); 
	  gst_object_unref(GST_OBJECT (pipeline));
	  return 1; }

	color = gst_element_factory_make ("ffmpegcolorspace",   "colorpsace");
	if (!color) { 
	  fputs("You need ffmpegcolorspace plugin installed.",stderr);
	  gst_object_unref(GST_OBJECT (pipeline));
	  return 1; }

	enc = gst_element_factory_make ("xvidenc", "xvid");
	if (!enc) { 
	  fputs("You need theoraenc plugin installed.",stderr);
	  gst_object_unref(GST_OBJECT (pipeline));
	  return 1; }

	mux = gst_element_factory_make ("avimux", "multiplex");
	if (!mux) {  
	  fputs("You need avimux plugin installed.", stderr);
	  gst_object_unref(GST_OBJECT (pipeline));
	  return 1; }

	queue2 = gst_element_factory_make ("queue",		 "queue2");
	if (!queue2) {
	  fputs("You need queue plugin installed.",stderr); 
	  gst_object_unref(GST_OBJECT (pipeline));
	  return 1; }

	sink = gst_element_factory_make ("filesink", "sink");
	if (!sink) {  
	  fputs("You need filesink plugin installed.",stderr);
	  gst_object_unref(GST_OBJECT (pipeline));
	  return 1; }


	GstCaps *caps;

	caps = gst_caps_new_simple ("video/x-raw-rgb",
	  	     "framerate", GST_TYPE_FRACTION, fps, 1,
	  	    // "width", G_TYPE_INT , 200,
	  	    // "height", G_TYPE_INT, 200,
	  	    // "pixel-aspect-ratio", GST_TYPE_FRACTION, 1 , 1,
		      NULL);

	gst_bin_add_many (GST_BIN (pipeline), source, queue, rate, color, enc, queue2, mux, sink, NULL);


	if (!gst_element_link_filtered (source, color, caps)) {
		  fputs("Filter link error.",stderr);
		  return 1;
	}

	if (!gst_element_link_many (color, queue, rate, enc, queue2, mux, sink, NULL)) {
	 		 fputs("Link many error.",stderr);
	 		 return 1;
	}


	int x=startX;
	int y=startY;
	int ex=endX;
	int ey=endY;
	
	// g_object_set (G_OBJECT(source), "num-buffers", 100, NULL);
	 g_object_set (G_OBJECT(source), "use-damage", true , NULL);
	 g_object_set (G_OBJECT(source), "startx", startX , NULL);
	 g_object_set (G_OBJECT(source), "starty", startY , NULL);
	 g_object_set (G_OBJECT(source), "endx", endX , NULL);
	 g_object_set (G_OBJECT(source), "endy", endY , NULL);

	 g_object_set (G_OBJECT(queue), "max-size-buffers", 25 , NULL);
	 g_object_set (G_OBJECT(queue), "max-size-time", 0 , NULL);
	 g_object_set (G_OBJECT(queue), "max-size-bytes", 0 , NULL);
	 g_object_set (G_OBJECT(queue), "min-threshold-buffers", 0 , NULL);
	 g_object_set (G_OBJECT(queue), "min-threshold-time", 0 , NULL);
	 g_object_set (G_OBJECT(queue), "min-threshold-bytes", 0 , NULL);

	 g_object_set (G_OBJECT(queue2), "max-size-buffers", 100 , NULL);
	 g_object_set (G_OBJECT(queue2), "max-size-time", 0 , NULL);
	 g_object_set (G_OBJECT(queue2), "max-size-bytes", 0 , NULL);
	 g_object_set (G_OBJECT(queue2), "min-threshold-buffers", 0 , NULL);
	 g_object_set (G_OBJECT(queue2), "min-threshold-time", 0 , NULL);
	 g_object_set (G_OBJECT(queue2), "min-threshold-bytes", 0 , NULL);
/*
	 g_object_set (G_OBJECT(enc), "drop-frames", false , NULL);
	 g_object_set (G_OBJECT(enc), "rate-buffer", 1000 , NULL);
	 g_object_set (G_OBJECT(enc), "keyframe-force", 1 , NULL);
	 g_object_set (G_OBJECT(enc), "keyframe-freq", 1 , NULL);
*/

	 //g_object_set (G_OBJECT(sink), "buffer-mode", 0 , NULL);
	 g_object_set (G_OBJECT (sink), "location", location,  NULL);
	 //g_object_set (G_OBJECT (sink), "sync", true, NULL);*/
	 // g_object_set (G_OBJECT (sink), "preroll-queue-len", 10, NULL);



	 gst_element_set_state (pipeline, GST_STATE_PLAYING);

	
	return 0;
}

int recordOgg(QString *path, int fps, int startX, int startY, int endX, int endY) {

	char *sta = strdup( path[0].toAscii().data() ); 
	char location[strlen(sta)+10];
	memset(location, 0, (strlen(sta)+10));
	strcpy(location,sta);
	strcat(location,"video.ogg");

	GstElement *source, *queue, *rate, *color, *queue2, *mux, *sink;
	
	pipeline = gst_pipeline_new ("desktop-recorder");
	if (!pipeline) { fputs("Pipeline error.",stderr); return 1; }

	source = gst_element_factory_make ("ximagesrc", "source");
	if (!source) { 
	  gst_object_unref(GST_OBJECT (pipeline));
	  return 1; }

	queue = gst_element_factory_make ("queue", "queue");
	if (!queue) { 
	  fputs("You need queue plugin installed.",stderr); 
	  gst_object_unref(GST_OBJECT (pipeline));
	  return 1; }

	rate = gst_element_factory_make ("videorate", "rate");
	if (!rate) { 
	  fputs("You need videorate plugin installed.",stderr); 
	  gst_object_unref(GST_OBJECT (pipeline));
	  return 1; }

	color = gst_element_factory_make ("ffmpegcolorspace",   "colorpsace");
	if (!color) { 
	  fputs("You need ffmpegcolorspace plugin installed.",stderr);
	  gst_object_unref(GST_OBJECT (pipeline));
	  return 1; }

	enc = gst_element_factory_make ("theoraenc", "theora");
	if (!enc) { 
	  fputs("You need theoraenc plugin installed.",stderr);
	  gst_object_unref(GST_OBJECT (pipeline));
	  return 1; }

	mux = gst_element_factory_make ("oggmux", "multiplex");
	if (!mux) {  
	  fputs("You need avimux plugin installed.", stderr);
	  gst_object_unref(GST_OBJECT (pipeline));
	  return 1; }

	queue2 = gst_element_factory_make ("queue",		 "queue2");
	if (!queue2) {
	  fputs("You need queue plugin installed.",stderr); 
	  gst_object_unref(GST_OBJECT (pipeline));
	  return 1; }

	sink = gst_element_factory_make ("filesink", "sink");
	if (!sink) {  
	  fputs("You need filesink plugin installed.",stderr);
	  gst_object_unref(GST_OBJECT (pipeline));
	  return 1; }


	GstCaps *caps;

	caps = gst_caps_new_simple ("video/x-raw-rgb",
	  	     "framerate", GST_TYPE_FRACTION, fps, 1,
	  	    // "width", G_TYPE_INT , 200,
	  	    // "height", G_TYPE_INT, 200,
	  	     "pixel-aspect-ratio", GST_TYPE_FRACTION, 1 , 1,
		      NULL);

	gst_bin_add_many (GST_BIN (pipeline), source, queue, color, enc, queue2, mux, sink, NULL);


	if (!gst_element_link_filtered (source, color, caps)) {
		  fputs("Filter link error.",stderr);
		  return 1;
	}

	if (!gst_element_link_many (color, queue, enc, queue2, mux, sink, NULL)) {
	 		 fputs("Link many error.",stderr);
	 		 return 1;
	}


	// g_object_set (G_OBJECT(source), "num-buffers", 100, NULL);
	 g_object_set (G_OBJECT(source), "use-damage", true , NULL);
	 g_object_set (G_OBJECT(source), "startx", startX , NULL);
	 g_object_set (G_OBJECT(source), "starty", startY , NULL);
	 g_object_set (G_OBJECT(source), "endx", endX , NULL);
	 g_object_set (G_OBJECT(source), "endy", endY , NULL);
	 
	 g_object_set (G_OBJECT(queue), "max-size-buffers", 50 , NULL);
	 g_object_set (G_OBJECT(queue), "max-size-time", 0 , NULL);
	 g_object_set (G_OBJECT(queue), "max-size-bytes", 0 , NULL);
	 g_object_set (G_OBJECT(queue), "min-threshold-buffers", 0 , NULL);
	 g_object_set (G_OBJECT(queue), "min-threshold-time", 0 , NULL);
	 g_object_set (G_OBJECT(queue), "min-threshold-bytes", 0 , NULL);
	 
	 g_object_set (G_OBJECT(queue), "max-size-buffers", 100 , NULL);
	 g_object_set (G_OBJECT(queue), "max-size-time", 0 , NULL);
	 g_object_set (G_OBJECT(queue), "max-size-bytes", 0 , NULL);
	 g_object_set (G_OBJECT(queue), "min-threshold-buffers", 0 , NULL);
	 g_object_set (G_OBJECT(queue), "min-threshold-time", 0 , NULL);
	 g_object_set (G_OBJECT(queue), "min-threshold-bytes", 0 , NULL);
/*
	 g_object_set (G_OBJECT(enc), "drop-frames", false , NULL);
	 g_object_set (G_OBJECT(enc), "rate-buffer", 1000 , NULL);
	 g_object_set (G_OBJECT(enc), "keyframe-force", 1 , NULL);
	 g_object_set (G_OBJECT(enc), "keyframe-freq", 1 , NULL);
*/

	 //g_object_set (G_OBJECT(sink), "buffer-mode", 0 , NULL);
	 g_object_set (G_OBJECT (sink), "location", location, NULL);
	 //g_object_set (G_OBJECT (sink), "sync", true, NULL);*/
	 // g_object_set (G_OBJECT (sink), "preroll-queue-len", 10, NULL);



	 gst_element_set_state (pipeline, GST_STATE_PLAYING);

	
	return 0;
}



int stopRec() {
	
	gst_element_send_event(pipeline, gst_event_new_flush_start());
	gst_element_send_event (enc, gst_event_new_eos ());
	gst_element_send_event(pipeline, gst_event_new_flush_stop());
	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(GST_OBJECT (pipeline));
	
	pipeline=NULL;
}

int display(QWidget *widget) {
  
	GstElement *source, *color, *sink, *queue;
  
  	pipeline2 = gst_pipeline_new ("desktop-recorder");
	if (!pipeline2) { fputs("Pipeline error.",stderr); return 1; }
	
	source = gst_element_factory_make ("ximagesrc",       "source");
	if (!source) { fputs("You need ximagesrc plugin installed.",stderr); return 1; }

	color = gst_element_factory_make ("ffmpegcolorspace",		 "color");
	if (!color) { fputs("You need queue plugin installed.",stderr); return 1; }
    
	queue = gst_element_factory_make("queue", "queue");
	if (!queue) { fputs("You need videoscale plugin installed.",stderr); return 1;}
    
	sink = gst_element_factory_make ("xvimagesink",		 "sink");
	if (!sink) { fputs("You need ximagesink plugin installed.",stderr); return 1; }
	
	
	gst_bin_add_many (GST_BIN (pipeline2), source, color, sink, queue,  NULL);

	GstCaps *caps;

	caps = gst_caps_new_simple ("video/x-raw-rgb",
	  	     "framerate", GST_TYPE_FRACTION, 10, 1,
	  	    // "width", G_TYPE_INT , 200,
	  	    // "height", G_TYPE_INT, 200,
	  	     "pixel-aspect-ratio", GST_TYPE_FRACTION, 1 , 1,
		      NULL);

			      
	if (!gst_element_link_filtered (source, color, caps)) {
		  fputs("Filter link error.",stderr);
		  return 1;
	}

	if (!gst_element_link_many (color, queue, sink, NULL)) {
	 		 fputs("Link many error.",stderr);
	 		 return 1;
	}
		 
	gst_x_overlay_prepare_xwindow_id(GST_X_OVERLAY(sink));
	
	gst_element_set_state(sink, GST_STATE_READY);
	gst_element_set_state (pipeline2, GST_STATE_PLAYING);
	
	QApplication::syncX();
	gst_x_overlay_set_xwindow_id(GST_X_OVERLAY(sink),widget->winId());
	
	
	return 0;
}

void pausedDisplay() {
  	
	gst_element_set_state(pipeline2, GST_STATE_PAUSED);
}

void unpausedDisplay()
{
  gst_element_set_state(pipeline2, GST_STATE_PLAYING);
}

void endGst()
{
 
  gst_element_send_event (pipeline2, gst_event_new_eos ());
  gst_element_set_state(pipeline2, GST_STATE_NULL);
  gst_object_unref(GST_OBJECT (pipeline2));
  if (pipeline!=NULL) { gst_object_unref(pipeline); }
  
}


