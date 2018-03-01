/*-------------------------------GPL-------------------------------------//
//
// MetOcean Viewer - A simple interface for viewing hydrodynamic model data
// Copyright (C) 2015  Zach Cobell
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
//-----------------------------------------------------------------------*/

#include <QApplication>
#include "MainWindow.h"
#include "movGeneric.h"

//-------------------------------------------//
// Main function for the code
//-------------------------------------------//
int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QString sessionFile;
  bool doSession;

  //...Display the splash screen
  QPixmap pixmap(":/rsc/img/logo_full.png");
  QSplashScreen splash(pixmap, Qt::WindowStaysOnTopHint);
  splash.setEnabled(false);
  splash.show();

  //...Check for the internet connection
  if (!MovGeneric::isConnectedToNetwork()) {
    QMessageBox::critical(nullptr,
                          QObject::tr("Internet Connection Not Detected"),
                          QObject::tr("No internet connection was detected.\n"
                                      "The program will now be terminated."));
    return 1;
  }

  // Check for drag/drop operations
  if (argc == 2) {
    sessionFile = QCoreApplication::arguments().at(1);
    doSession = true;
  } else {
    doSession = false;
    sessionFile = QString();
  }

  // Create the window
  MainWindow w(doSession, sessionFile);

  // Hide the splash screen
  splash.hide();

  // Show the program window
  w.show();

  return a.exec();
}
//-------------------------------------------//
