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


#include "KUI_project.h"

GstElement *pipeline, *pipeline2;

int record(char *location) {

	
	GstElement *source, *queue, *rate, *color, *enc, *queue2, *mux, *sink;


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
	  	     "framerate", GST_TYPE_FRACTION, 15, 1,
	  	    // "width", G_TYPE_INT , 200,
	  	    // "height", G_TYPE_INT, 200,
	  	     "pixel-aspect-ratio", GST_TYPE_FRACTION, 1 , 1,
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


	// g_object_set (G_OBJECT(source), "num-buffers", 100, NULL);
	 g_object_set (G_OBJECT(source), "use-damage", true , NULL);
	/* g_object_set (G_OBJECT(source), "startx", 0 , NULL);
	 g_object_set (G_OBJECT(source), "starty", 0 , NULL);
	 g_object_set (G_OBJECT(source), "endx", 400 , NULL);
	 g_object_set (G_OBJECT(source), "endy", 400 , NULL);
*/
	 g_object_set (G_OBJECT(queue), "max-size-buffers", 25 , NULL);
	 g_object_set (G_OBJECT(queue), "max-size-time", 0 , NULL);
	 g_object_set (G_OBJECT(queue), "max-size-bytes", 0 , NULL);
	 g_object_set (G_OBJECT(queue), "min-threshold-buffers", 10 , NULL);
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
	
	gst_element_send_event (pipeline, gst_event_new_eos ());
	//sleep(5);
	gst_element_set_state(pipeline, GST_STATE_NULL);
	gst_object_unref(GST_OBJECT (pipeline));
}

int display(QWidget *widget) {
  
	GstElement *source, *color, *sink;
  
  	pipeline2 = gst_pipeline_new ("desktop-recorder");
	if (!pipeline2) { fputs("Pipeline error.",stderr); return 1; }
	
	source = gst_element_factory_make ("ximagesrc",       "source");
	if (!source) { fputs("You need ximagesrc plugin installed.",stderr); return 1; }

	color = gst_element_factory_make ("ffmpegcolorspace",		 "color");
	if (!color) { fputs("You need queue plugin installed.",stderr); return 1; }
		
	sink = gst_element_factory_make ("ximagesink",		 "sink");
	if (!sink) { fputs("You need queue plugin installed.",stderr); return 1; }
	
	
	gst_bin_add_many (GST_BIN (pipeline2), source, color, sink, NULL);

	
	if (!gst_element_link_many (source, color, sink, NULL)) {
	 		 fputs("Link many error.",stderr);
	 		 return 1;
	}
		 
	gst_element_set_state(sink, GST_STATE_READY);
	gst_element_set_state (pipeline2, GST_STATE_PLAYING);
	
	gst_x_overlay_set_xwindow_id(GST_X_OVERLAY(sink),widget->winId());
	
	return 0;
}
