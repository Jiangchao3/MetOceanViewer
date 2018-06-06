/*-------------------------------GPL-------------------------------------//
//
// MetOcean Viewer - A simple interface for viewing hydrodynamic model data
// Copyright (C) 2018  Zach Cobell
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
#include "xtide.h"
#include <float.h>
#include "hmdf.h"

//...Constructor
XTide::XTide(QQuickWidget *inMap, ChartView *inChart,
             QDateEdit *inStartDateEdit, QDateEdit *inEndDateEdit,
             QComboBox *inUnits, QStatusBar *inStatusBar,
             StationModel *inStationModel, QString *inCurrentStation,
             QObject *parent)
    : QObject(parent) {
  this->m_quickMap = inMap;
  this->m_chartView = inChart;
  this->m_startDateEdit = inStartDateEdit;
  this->m_endDateEdit = inEndDateEdit;
  this->m_comboUnits = inUnits;
  this->m_statusBar = inStatusBar;
  this->m_station.setLatitude(0.0);
  this->m_station.setLongitude(0.0);
  this->m_station.name() = QString();
  this->m_currentStation = inCurrentStation;
  this->m_stationModel = inStationModel;
}

//...Destructor
XTide::~XTide() {}

QString XTide::getErrorString() { return this->m_errorString; }

ChartView *XTide::chartview() { return this->m_chartView; }

//...Overall routine for plotting XTide
int XTide::plotXTideStation() {
  int ierr;

  //...Get the executables
  ierr = this->findXTideExe();
  if (ierr != 0) return -1;

  //...Get the selected station
  this->m_station =
      this->m_stationModel->findStation(*(this->m_currentStation));

  //...Calculate the tide signal
  ierr = this->calculateXTides();
  if (ierr == 0) this->plotChart();

  return 0;
}

QString XTide::getLoadedXTideStation() { return this->m_station.id(); }

QString XTide::getCurrentXTideStation() { return *(this->m_currentStation); }

//...Find the xTide executable
int XTide::findXTideExe() {
  QString installLocation =
      QApplication::applicationDirPath().replace(" ", "\ ");
  QString buildLocationLinux =
      QApplication::applicationDirPath() +
      "/../../MetOceanViewer/MetOceanViewer_GUI/mov_libs/bin";
  QString buildLocationWindows =
      QApplication::applicationDirPath() +
      "/../../../MetOceanViewer/thirdparty/xtide-2.15.1";
  QString appLocationMacOSX = QApplication::applicationDirPath();

  QFile location1(installLocation + "/tide");
  QFile location2(buildLocationLinux + "/tide");
  QFile location3(buildLocationWindows + "/tide.exe");
  QFile location4(buildLocationLinux + "/tide.exe");
  QFile location5(installLocation + "/tide.exe");
  QFile location6(appLocationMacOSX + "/XTide/bin/tide");

  if (location1.exists()) {
    this->m_xtideexe = installLocation + "/tide";
    this->m_harmfile = installLocation + "/harmonics.tcd";
    return 0;
  }

  if (location2.exists()) {
    this->m_xtideexe = buildLocationLinux + "/tide";
    this->m_harmfile = buildLocationLinux + "/harmonics.tcd";
    return 0;
  }

  if (location3.exists()) {
    this->m_xtideexe = buildLocationWindows + "/tide";
    this->m_harmfile = buildLocationWindows + "/harmonics.tcd";
    return 0;
  }

  if (location4.exists()) {
    this->m_xtideexe = buildLocationLinux + "/tide.exe";
    this->m_harmfile = buildLocationLinux + "/harmonics.tcd";
    return 0;
  }

  if (location5.exists()) {
    this->m_xtideexe = installLocation + "/tide.exe";
    this->m_harmfile = installLocation + "/harmonics.tcd";
    return 0;
  }

  if (location6.exists()) {
    this->m_xtideexe = appLocationMacOSX + "/XTide/bin/tide";
    this->m_harmfile = appLocationMacOSX + "/XTide/bin/harmonics.tcd";
    return 0;
  }

  emit xTideError(tr("The XTide executable was not found"));

  return -1;
}

//...Compute the tidal signal
int XTide::calculateXTides() {
  int ierr;
  QEventLoop loop;

  //...Get the selected dates
  QDateTime startDate = this->m_startDateEdit->dateTime();
  QDateTime endDate = this->m_endDateEdit->dateTime();
  startDate.setTime(QTime(0, 0, 0));
  endDate = endDate.addDays(1);
  endDate.setTime(QTime(0, 0, 0));
  QString startDateString = startDate.toString("yyyy-MM-dd hh:mm");
  QString endDateString = endDate.toString("yyyy-MM-dd hh:mm");

  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("HFILE_PATH", this->m_harmfile);

//...Build a calling string. For windows, quote the executable
//   to avoid issues with path names like "Program Files". We'll
//   assume Linux users aren't dumb enough to do such a thing
#ifdef _WIN32
  QString xTideCmd = "\"" + this->m_xtideexe.replace(" ", "\ ") +
                     "\""
                     " -l \"" +
                     this->m_station.name() + "\"" + " -b \"" +
                     startDateString + "\"" + " -e \"" + endDateString + "\"" +
                     " -s \"00:30\" -z -m m";
#else
  QString xTideCmd = this->m_xtideexe + " -l \"" + this->m_station.name() +
                     "\"" + " -b \"" + startDateString + "\"" + " -e \"" +
                     endDateString + "\"" + " -s \"00:30\" -z -m m";
#endif

  QProcess xTideRun(this);
  xTideRun.setEnvironment(env.toStringList());
  xTideRun.start(xTideCmd);
  connect(&xTideRun, SIGNAL(finished(int, QProcess::ExitStatus)), &loop,
          SLOT(quit()));
  loop.exec();

  //...Check the error code
  ierr = xTideRun.exitCode();
  if (ierr != 0) return -1;

  //...Grab the output from XTide and send to the parse routine
  ierr = this->parseXTideResponse(xTideRun.readAllStandardOutput());

  return ierr;
}

int XTide::parseXTideResponse(QString xTideResponse) {
  QString tempString, tempDate, tempTime, tempAMPM, tempElev;
  QString tempHour, tempMinute;
  QStringList tempList;
  QStringList response = xTideResponse.split("\n");
  QDate date;
  QTime time;
  XTideStationData thisData;
  int hour, minute;
  double elevation, unitConvert;

  if (this->m_comboUnits->currentIndex() == 0)
    unitConvert = 1.0 / 3.28084;
  else
    unitConvert = 1.0;

  this->m_stationData.clear();

  for (int i = 0; i < response.length(); i++) {
    tempString = response.value(i);
    tempString = tempString.simplified();
    tempList = tempString.split(" ");

    if (tempList.length() != 5) continue;

    tempDate = tempList.value(0);
    tempTime = tempList.value(1);
    tempAMPM = tempList.value(2);
    tempElev = tempList.value(4);

    tempHour = tempTime.split(":").value(0);
    tempMinute = tempTime.split(":").value(1);
    hour = tempHour.toInt();
    minute = tempMinute.toInt();

    if (tempAMPM == "PM")
      if (hour != 12)
        hour = hour + 12;
      else
        hour = 12;
    else if (hour == 12)
      hour = 0;

    date = QDate::fromString(tempDate, "yyyy-MM-dd");
    time = QTime(hour, minute);
    elevation = tempElev.toDouble();

    thisData.m_date = QDateTime(date, time).toMSecsSinceEpoch();
    thisData.m_value = elevation * unitConvert;

    this->m_stationData.push_back(thisData);
  }

  return 0;
}

int XTide::getDataBounds(double &min, double &max) {
  int j;

  min = DBL_MAX;
  max = -DBL_MAX;

  for (j = 0; j < this->m_stationData.length(); j++) {
    if (this->m_stationData[j].m_value < min)
      min = this->m_stationData[j].m_value;
    if (this->m_stationData[j].m_value > max)
      max = this->m_stationData[j].m_value;
  }
  return 0;
}

int XTide::plotChart() {
  double ymin, ymax;
  QString format;
  QDateTime minDateTime, maxDateTime, startDate, endDate;

  maxDateTime = QDateTime(QDate(1000, 1, 1), QTime(0, 0, 0));
  minDateTime = QDateTime(QDate(3000, 1, 1), QTime(0, 0, 0));

  startDate = this->m_startDateEdit->dateTime();
  endDate = this->m_endDateEdit->dateTime();

  int ierr = this->getDataBounds(ymin, ymax);
  if (ierr != 0) return ierr;

  if (this->m_comboUnits->currentIndex() == 1)
    this->m_ylabel = tr("Water Surface Elevation (ft, MLLW)");
  else
    this->m_ylabel = tr("Water Surface Elevation (m, MLLW)");

  //...Create the chart
  this->m_chartView->m_chart = new QChart();

  QLineSeries *series1 = new QLineSeries(this);
  series1->setName(this->m_station.name());
  series1->setPen(
      QPen(QColor(0, 255, 0), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

  minDateTime = this->m_startDateEdit->dateTime();
  maxDateTime = this->m_endDateEdit->dateTime().addDays(1);
  minDateTime.setTime(QTime(0, 0, 0));
  maxDateTime.setTime(QTime(0, 0, 0));

  QDateTimeAxis *axisX = new QDateTimeAxis(this);
  axisX->setTickCount(5);
  if (startDate.daysTo(endDate) > 90)
    axisX->setFormat("MM/yyyy");
  else if (startDate.daysTo(endDate) > 4)
    axisX->setFormat("MM/dd/yyyy");
  else
    axisX->setFormat("MM/dd/yyyy hh:mm");
  axisX->setTitleText("Date (GMT)");
  axisX->setTitleFont(QFont("Helvetica", 10, QFont::Bold));
  axisX->setMin(minDateTime);
  axisX->setMax(maxDateTime);
  this->m_chartView->m_chart->addAxis(axisX, Qt::AlignBottom);

  QValueAxis *axisY = new QValueAxis(this);
  axisY->setLabelFormat(format);
  axisY->setTitleText(this->m_ylabel);
  axisY->setTitleFont(QFont("Helvetica", 10, QFont::Bold));
  axisY->setMin(ymin);
  axisY->setMax(ymax);
  this->m_chartView->m_chart->addAxis(axisY, Qt::AlignLeft);

  for (int i = 0; i < this->m_stationData.length(); i++)
    series1->append(this->m_stationData[i].m_date,
                    this->m_stationData[i].m_value);
  this->m_chartView->m_chart->addSeries(series1);
  this->m_chartView->clear();
  this->m_chartView->addSeries(series1, series1->name());
  series1->attachAxis(axisX);
  series1->attachAxis(axisY);
  axisY->setTickCount(10);
  axisY->applyNiceNumbers();
  axisX->setGridLineColor(QColor(200, 200, 200));
  axisY->setGridLineColor(QColor(200, 200, 200));
  axisY->setShadesPen(Qt::NoPen);
  axisY->setShadesBrush(QBrush(QColor(240, 240, 240)));
  axisY->setShadesVisible(true);

  this->m_chartView->m_chart->setAnimationOptions(QChart::SeriesAnimations);
  this->m_chartView->m_chart->legend()->setAlignment(Qt::AlignBottom);
  this->m_chartView->m_chart->setTitle("XTide Station: " +
                                       this->m_station.name());
  this->m_chartView->m_chart->setTitleFont(QFont("Helvetica", 14, QFont::Bold));
  this->m_chartView->setRenderHint(QPainter::Antialiasing);
  this->m_chartView->setChart(this->m_chartView->m_chart);

  this->m_chartView->m_style = 1;
  this->m_chartView->m_coord =
      new QGraphicsSimpleTextItem(this->m_chartView->m_chart);
  this->m_chartView->m_coord->setPos(
      this->m_chartView->size().width() / 2 - 100,
      this->m_chartView->size().height() - 20);
  this->m_chartView->initializeAxisLimits();
  this->m_chartView->setStatusBar(this->m_statusBar);
  this->m_chartView->m_chart->legend()->markers().at(0)->setFont(
      QFont("Helvetica", 10, QFont::Bold));

  foreach (QLegendMarker *marker,
           this->m_chartView->m_chart->legend()->markers()) {
    // Disconnect possible existing connection to avoid multiple connections
    QObject::disconnect(marker, SIGNAL(clicked()), this->m_chartView,
                        SLOT(handleLegendMarkerClicked()));
    QObject::connect(marker, SIGNAL(clicked()), this->m_chartView,
                     SLOT(handleLegendMarkerClicked()));
  }

  return 0;
}

int XTide::saveXTideData(QString filename, QString format) {
  Hmdf *xtideOut = new Hmdf(this);
  HmdfStation *station = new HmdfStation(xtideOut);

  station->setLongitude(this->m_station.coordinate().longitude());
  station->setLatitude(this->m_station.coordinate().latitude());
  station->setName(this->m_station.name());
  station->setName(this->m_station.name().replace(" ", "_"));
  station->setId(this->m_station.id());
  station->setStationIndex(1);
  for (int i = 0; i < this->m_stationData.length(); i++) {
    station->setNext(this->m_stationData[i].m_date,
                     this->m_stationData[i].m_value);
  }

  xtideOut->addStation(station);
  int ierr = xtideOut->write(filename);
  if (ierr != 0) {
    emit xTideError("Error writing XTide data to file.");
  }

  delete xtideOut;

  return 0;
}

int XTide::saveXTidePlot(QString filename, QString filter) {
  if (filter == "PDF (*.pdf)") {
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPrinter::Letter);
    printer.setResolution(400);
    printer.setOrientation(QPrinter::Landscape);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filename);

    QPainter painter(&printer);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.begin(&printer);

    //...Page 1 - Chart
    this->m_chartView->render(&painter);

    //...Page 2 - Map
    printer.newPage();
    QPixmap renderedMap = this->m_quickMap->grab();
    QPixmap mapScaled = renderedMap.scaledToWidth(printer.width());
    if (mapScaled.height() > printer.height())
      mapScaled = renderedMap.scaledToHeight(printer.height());
    int cw = (printer.width() - mapScaled.width()) / 2;
    int ch = (printer.height() - mapScaled.height()) / 2;
    painter.drawPixmap(cw, ch, mapScaled.width(), mapScaled.height(),
                       mapScaled);

    painter.end();
  } else if (filter == "JPG (*.jpg *.jpeg)") {
    QFile outputFile(filename);
    QSize imageSize(
        this->m_quickMap->size().width() + this->m_chartView->size().width(),
        this->m_quickMap->size().height());
    QRect chartRect(this->m_quickMap->size().width(), 0,
                    this->m_chartView->size().width(),
                    this->m_chartView->size().height());

    QImage pixmap(imageSize, QImage::Format_ARGB32);
    pixmap.fill(Qt::white);
    QPainter imagePainter(&pixmap);
    imagePainter.setRenderHints(QPainter::Antialiasing |
                                QPainter::TextAntialiasing |
                                QPainter::SmoothPixmapTransform);
    this->m_quickMap->render(&imagePainter, QPoint(0, 0));
    this->m_chartView->render(&imagePainter, chartRect);

    outputFile.open(QIODevice::WriteOnly);
    pixmap.save(&outputFile, "JPG", 100);
  }

  return 0;
}
