/*-------------------------------GPL-------------------------------------//
//
// MetOcean Viewer - A simple interface for viewing hydrodynamic model data
// Copyright (C) 2019  Zach Cobell
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
#include "generic.h"

#include "projection.h"
#include <QDesktopServices>
#include <QFileInfo>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "boost/algorithm/string.hpp"

static std::string c_configDirectory = std::string();

//-------------------------------------------//
// Simple delay function which will pause
// execution for a number of seconds
//-------------------------------------------//
void Generic::delay(int delayTime) {
  QTime dieTime = QTime::currentTime().addSecs(delayTime);
  while (QTime::currentTime() < dieTime)
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
  return;
}
//-------------------------------------------//

//-------------------------------------------//
// Simple delay function which will pause
// execution for a number of milliseconds
//-------------------------------------------//
void Generic::delayM(int delayTime) {
  QTime dieTime = QTime::currentTime().addMSecs(delayTime);
  while (QTime::currentTime() < dieTime)
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
//-------------------------------------------//

void Generic::splitPath(const std::string &input, std::string &filename,
                        std::string &directory) {
  QFileInfo in(QString::fromStdString(input));
  filename = in.fileName().toStdString();
  directory = in.absoluteDir().absolutePath().toStdString();
}

//-------------------------------------------//

//-------------------------------------------//
// Function that checks if the computer is
// connected to the network. This is set as a
// prerequisite for running the program since
// every page is Google Maps based, and requires
// a connection to the internet
//-------------------------------------------//
bool Generic::isConnectedToNetwork() {
  QNetworkAccessManager nam;
  QNetworkRequest req(QUrl("http://www.google.com"));
  QNetworkReply *reply = nam.get(req);
  QEventLoop loop;
  QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
  loop.exec();
  if (reply->bytesAvailable())
    return true;
  else
    return false;
}

// bool Generic::createConfigDirectory(const std::string &configDirectory) {
bool Generic::createConfigDirectory() {
  c_configDirectory =
      QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation)
          .at(0)
          .toStdString();
  if (!QDir(QString::fromStdString(c_configDirectory)).exists()) {
    return QDir().mkpath(QString::fromStdString(c_configDirectory));
  } else {
    return true;
  }
}

std::string Generic::configDirectory() {
  return QStandardPaths::standardLocations(QStandardPaths::AppConfigLocation)
      .at(0)
      .toStdString();
}

std::string Generic::crmsDataFile() {
  return Generic::configDirectory() + "/crms.nc";
}

void Generic::findProjDatabase() {
  std::string db_location;
  bool db_found = false;
  QDirIterator it(QDir::currentPath(), QDirIterator::Subdirectories);
  while (it.hasNext()) {
    auto filename = it.next();
    auto file = QFileInfo(filename);
    if (file.isDir()) {
      continue;
    }
    if (file.fileName().contains("proj.db", Qt::CaseInsensitive)) {
      db_found = true;
      db_location = file.absoluteFilePath().toStdString();
      break;
    }
  }
  if (db_found)
    Hmdf::Projection::setProjDatabaseLocation(db_location);
}
