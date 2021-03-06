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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QActionGroup>
#include <QColorDialog>
#include <QFileDialog>
#include <QMainWindow>
#include <QNetworkInterface>
#include <QQuickWidget>
#include <QTimer>
#include <QUrl>
#include <QVector>
#include <QtCharts>
#include <QtNetwork>
#include "addtimeseriesdialog.h"
#include "mapfunctions.h"
#include "stationmodel.h"

//...Forward declarations of classes
class Noaa;
class Usgs;
class Hwm;
class XTide;
class Ndbc;
class UserTimeseries;
class WebEnginePage;
class Session;
class Crms;

//-------------------------------------------//
// Main class used by the Qt program that holds
// the main set of functions used
//-------------------------------------------//
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(bool processCommandLine, QString commandLineFile,
                      QWidget *parent = nullptr);

  ~MainWindow();

  void setLoadSessionFile(bool toggle, QString sessionFile = QString());

  QString previousDirectory;

 private slots:

  void throwErrorMessageBox(QString);

  void setupMetOceanViewerUI();

  void on_Button_FetchData_clicked();

  void on_combo_noaa_maptype_currentIndexChanged(int index);

  void on_button_noaasavechart_clicked();

  void on_button_noaasavedata_clicked();

  void on_button_saveTimeseriesImage_clicked();

  void on_check_TimeseriesYauto_toggled(bool checked);

  void on_browse_hwm_clicked();

  void on_button_processHWM_clicked();

  void on_check_manualHWM_toggled(bool checked);

  void on_button_saveHWMMap_clicked();

  void on_button_hwmcolor_clicked();

  void on_button_121linecolor_clicked();

  void on_button_reglinecolor_clicked();

  void on_button_boundlinecolor_clicked();

  void on_actionQuit_triggered();

  void on_button_TimeseriesDeleteRow_clicked();

  void on_button_TimeseriesAddRow_clicked();

  void on_button_TimeseriesCopyRow_clicked();

  void on_button_TimeseriesEditRow_clicked();

  void on_button_processTimeseriesData_clicked();

  void on_check_TimeseriesAllData_toggled(bool checked);

  void on_button_fitHWM_clicked();

  void on_button_plotTimeseriesStation_clicked();

  void on_actionAbout_triggered();

  void on_actionLoad_Session_triggered();

  void on_actionSave_Session_triggered();

  void on_actionSave_Session_As_triggered();

  void on_combo_usgs_maptype_currentIndexChanged(int index);

  void on_button_usgs_fetch_clicked();

  void on_button_usgssavemap_clicked();

  void on_button_usgssavedata_clicked();

  void on_combo_USGSProduct_currentIndexChanged(int index);

  void on_radio_usgsDaily_clicked();

  void on_radio_usgshistoric_clicked();

  void on_radio_usgs_instant_clicked();

  void on_check_regressionColorMatch_clicked(bool checked);

  void on_button_moveRowUp_clicked();

  void on_button_moveRowDown_clicked();

  void handleEnterKey();

  void on_button_saveHWMScatter_clicked();

  void on_button_noaaresetzoom_clicked();

  void on_button_usgsresetzoom_clicked();

  void on_button_usertimeseriesResetZoom_clicked();

  void on_button_hwmResetZoom_clicked();

  void on_actionCheck_For_Updates_triggered();

  void on_button_xtide_compute_clicked();

  void on_button_xtide_resetzoom_clicked();

  void on_button_xtide_savemap_clicked();

  void on_button_xtide_savedata_clicked();

  void on_combo_noaaTimezoneLocation_currentIndexChanged(int index);

  void on_combo_usgsTimezoneLocation_currentIndexChanged(int index);

  void on_combo_noaaTimezone_currentIndexChanged(const QString &arg1);

  void on_combo_usgsTimezone_currentIndexChanged(const QString &arg1);

  void changeNoaaMarker(QString markerId);

  void changeUsgsMarker(QString markerId);

  void changeXtideMarker(QString markerId);

  void changeUserMarker(QString markerId);

  void changeNdbcMarker(QString markerId);

  void changeCrmsMarker(QString markerId);

  void changeNoaaMaptype();

  void changeUsgsMaptype();

  void changeXtideMaptype();

  void changeUserMaptype();

  void changeHwmMaptype();

  void changeNdbcMaptype();

  void changeCrmsMaptype();

  void on_combo_xtide_maptype_currentIndexChanged(int index);

  void on_combo_user_maptype_currentIndexChanged(int index);

  void on_button_usertimeseries_fitMarkers_clicked();

  void on_button_noaaDisplayValues_toggled(bool checked);

  void on_button_usgsDisplayValues_toggled(bool checked);

  void on_button_xtideDisplayValues_toggled(bool checked);

  void on_button_timeseriesDisplayValues_toggled(bool checked);

  void on_button_hwmDisplayValues_toggled(bool checked);

  void on_combo_hwmMaptype_currentIndexChanged(int index);

  void on_actionESRI_toggled(bool arg1);

  void on_actionMapBox_toggled(bool arg1);

  void on_actionEnter_MapBox_API_Key_triggered();

  void on_actionSave_Default_Map_Settings_triggered();

  void on_actionOpenStreetMap_toggled(bool arg1);

  void on_button_fetchndbc_clicked();

  void on_combo_ndbcproduct_currentIndexChanged(int index);

  void on_button_ndbc_resetzoom_clicked();

  void on_button_ndbcDisplayValues_toggled(bool checked);

  void on_button_ndbc_savechart_clicked();

  void on_button_ndbc_savedata_clicked();

  void on_actionGenerate_CRMS_Database_triggered();

  void on_button_fetchcrms_clicked();

  void on_combo_crmsproduct_currentIndexChanged(int index);

  void on_combo_crms_maptype_currentIndexChanged(int index);

  void on_button_crms_savechart_clicked();

  void on_button_crms_savedata_clicked();

  void on_button_crms_resetzoom_clicked();

  void on_button_crmsDisplayValues_toggled(bool checked);

  void on_button_crmsfilterStationAvailablity_toggled(bool checked);

  void on_date_crmsStarttime_dateChanged(const QDate &date);

  void on_date_crmsEndtime_dateChanged(const QDate &date);

  void on_check_noaa_usevdatum_clicked(bool checked);

  void refreshNoaaStations();

  void updateNoaaStations();

  void refreshNdbcStations();

  void updateNdbcStations();

  void refreshUsgsStations();

  void updateUsgsStations();

  void refreshXTideStations();

  void updateXTideStations();

  void on_check_noaaActiveOnly_toggled(bool checked);

  private:
  enum MapViewerMarkerModes {
    SingleSelect,
    MultipleSelect,
    ColoredMarkers,
    SingleSelectWithDates
  };

  Ui::MainWindow *ui;

  QList<QTableWidgetItem *> grabTableRow(int row);

  void setTableRow(int row, const QList<QTableWidgetItem *> &rowItems);

  void setupTimeseriesTable();

  void setupNoaaMap();

  void setupUsgsMap();

  void setupXTideMap();

  void setupMarkerClasses(QQuickWidget *widget);

  void setupUserTimeseriesMap();

  void setupNdbcMap();

  void setupHighWaterMarkMap();

  void setupCrmsMap();

  void setupRandomColors();

  void setHwmMarkerCategories();

  void checkForUpdate();

  void installKeyhandlers();

  void parseCommandLine();

  void initializeSessionHandler();

  bool confirmClose();

  void plotNOAAStation();

  void plotXTideStation();

  void stationDisplayWarning(int n);

  void setTimeseriesTableRow(int row, AddTimeseriesDialog *dialog);

  void resetMapSource(MapFunctions::MapSource source);

  Noaa *m_noaa;

  Usgs *m_usgs;

  Hwm *m_hwm;

  XTide *m_xtide;

  Ndbc *m_ndbc;

  Crms *m_crms;

  UserTimeseries *m_userTimeseries;

  Session *sessionState;

  QString sessionFile;

  QVector<QColor> randomColors;

  QString commandLineFile;

  StationModel *noaaStationModel;

  StationModel *usgsStationModel;

  StationModel *xtideStationModel;

  StationModel *userDataStationModel;

  StationModel *hwmMarkerModel;

  StationModel *ndbcStationModel;

  StationModel *crmsStationModel;

  MapFunctions *mapFunctions;

  QActionGroup *mapActionGroup;

  QVector<QString> crmsHeader;
  QMap<QString, size_t> crmsMapping;

  QVector<Station> xtideMarkerLocations;
  QVector<Station> ndbcMarkerLocations;
  QVector<Station> noaaMarkerLocations;
  QVector<Station> usgsMarkerLocations;
  QVector<Station> crmsMarkerLocations;

  QString noaaSelectedStation;
  QString ndbcSelectedStation;
  QString usgsSelectedStation;
  QString xtideSelectedStation;
  QString userSelectedStations;
  QString crmsSelectedStations;

  QTimer *m_noaaDelayTimer;
  QTimer *m_usgsDelayTimer;
  QTimer *m_xtideDelayTimer;
  QTimer *m_ndbcDelayTimer;

  bool processCommandLine;
  bool initialized;

  void filterCrmsStationsByDate();

 protected:
  void closeEvent(QCloseEvent *);
};

#endif  // MAINWINDOW_H
