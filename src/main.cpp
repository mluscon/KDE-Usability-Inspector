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

#include <KApplication>
#include <KAboutData>
#include <KCmdLineArgs>

#include "KUI_mainToolBar.h"
#include "KUI_project.h"
 
int main (int argc, char *argv[])
{
  KAboutData aboutData( "KUI_project", "KUI_project",
      ki18n("KUI_project"), "1.0",
      ki18n("Usabality inspector"),
      KAboutData::License_GPL,
      ki18n("Copyright (c) 2007 Michal Luscon") );
  KCmdLineArgs::init( argc, argv, &aboutData );
  KApplication app;
 
  KUI_project *mainwindow= new KUI_project;
  
  mainwindow->show();
  
  return app.exec();
  
}
