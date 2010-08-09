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


#include <stdio.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <iostream>

#define NIL (0)

int selectWindow(unsigned int *startX, unsigned int *startY, unsigned int *endX, unsigned int *endY)
{
  
  Display *dpy = XOpenDisplay(NIL);
  int descend = 0;
  int status;
  Cursor cursor;
  XEvent event;
  Window target_win = None, root = RootWindow(dpy,NIL);
  int buttons = 0;

  /* Make the target cursor */
  cursor = XCreateFontCursor(dpy, XC_crosshair);

  /* Grab the pointer using target cursor, letting it room all over */
  status = XGrabPointer(dpy, root, False,
			ButtonPressMask|ButtonReleaseMask, GrabModeSync,
			GrabModeAsync, root, cursor, CurrentTime);
  

  /* Let the user select a window... */
  while ((target_win == None) || (buttons != 0)) {
    /* allow one more event */
    XAllowEvents(dpy, SyncPointer, CurrentTime);
    XWindowEvent(dpy, root, ButtonPressMask|ButtonReleaseMask, &event);
    switch (event.type) {
    case ButtonPress:
      if (target_win == None) {
	target_win = event.xbutton.subwindow; /* window selected */
	if (target_win == None) target_win = root;
      }
      buttons++;
      break;
    case ButtonRelease:
      if (buttons > 0) /* there may have been some down before we started */
	buttons--;
       break;
    }
  } 

  XUngrabPointer(dpy, CurrentTime);      /* Done with pointer */


  
  XWindowAttributes selWin;
  XGetWindowAttributes(dpy, target_win, &selWin);
  
  *startX = selWin.x;
  *startY = selWin.y;
  *endX = selWin.x + selWin.width;
  *endY = selWin.y + selWin.height;
  
  if (*startX%2==1) { *startX=*startX-1; }
  if (*startY%2==1) { *startY=*startY-1; }
  if (*endX%2==1) { *endX=*endX+1; }
  if (*endY%2==1) { *endY=*endY-1; }
      
  return 0;
}