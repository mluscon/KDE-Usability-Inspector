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

#ifndef KUI_project_H
#define KUI_project_H
  
#include <QWidget>
#include <qspinbox.h>

class QPushButton;
class QLineEdit;
class QStringList;
class QFileDialog;
class QRadioButton;
class QSpinBox;
class QLabel;

class KUI_project : public QWidget
{
  Q_OBJECT
  
  public:
    KUI_project(QWidget *parent=0);
    virtual ~KUI_project();
    QWidget *screenShotLabel;
    
  private:
    int fps;
    unsigned int sX, sY, eX, eY;
    bool avi;
    
    QPushButton *recordButton;
    QPushButton *selectButton;
    QPushButton *locationButton;
    QPushButton *stopButton;
    
    QRadioButton *aviButton;
    QRadioButton *oggButton;
    
    QSpinBox *fpsChanger;
    
    QLabel *fpsLabel;
    
    QLineEdit *address;
    QStringList path;
    
    QFileDialog *fileDiag;
    
  
  public slots:
    void winSelect();
    void location();
    void pathChenged();
    void startRecording();
    void stopRecording();
    void aviChecked();
    void oggChecked();
    void fpsChanged();
};


#endif // KUI_project_H
