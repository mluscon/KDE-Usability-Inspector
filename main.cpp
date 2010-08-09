
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
#include <QApplication>

#include "KUI_project.h"
#include "KUI_record.h"
#include "KUI_select.h"

int main (int argc, char *argv[])
{
  
  
  QApplication app(argc, argv);
  

  gst_init(&argc, &argv); 
  
  
  KUI_project kui;  
  kui.show();
  display(kui.screenShotLabel);
  
  return app.exec();
  
 

  
  return 0;
  
}
