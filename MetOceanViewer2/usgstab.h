#ifndef USGSTAB_H
#define USGSTAB_H

#include <QButtonGroup>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QVBoxLayout>
#include <tuple>
#include <vector>

#include "combobox.h"
#include "datebox.h"
#include "mapchartwidget.h"

class UsgsTab : public MapChartWidget {
  Q_OBJECT
 public:
  UsgsTab(QVector<Station> *stations, QWidget *parent = nullptr);

 private slots:
  void plot() override;
  void replot(int);
  void refreshStations();

 private:
  QGroupBox *generateInputBox() override;
  void connectSignals() override;
  int calculateDateInfo(QDateTime &startDate, QDateTime &endDate,
                        QDateTime &startDateGmt, QDateTime &endDateGmt,
                        QString &timezoneString, qint64 &tzOffset);
  void setPlotAxis(Hmdf *data, const QDateTime &startDate,
                   const QDateTime &endDate, const QString &tzAbbrev,
                   const QString &unitString, const QString &productName);
  std::tuple<QString, QString> splitUsgsProductName(const QString &product);
  void addSeriesToChart(int const index, const QString &name, const qint64 tzoffset);
  int getDatabaseType();

  QPushButton *m_btn_fetch;
  QPushButton *m_btn_refresh;
  QLabel *m_lbl_buttonGroup;
  QRadioButton *m_rbtn_historic;
  QRadioButton *m_rbtn_instant;
  QRadioButton *m_rbtn_daily;
  ComboBox *m_cbx_product;
  ComboBox *m_cbx_timezone;
  ComboBox *m_cbx_mapType;
  DateBox *m_dte_startDate;
  DateBox *m_dte_endDate;
  QButtonGroup *m_buttonGroup;
  std::vector<QHBoxLayout *> m_rowLayouts;
  std::unique_ptr<Hmdf> m_data;
  Station m_currentStation;
  bool m_ready;
};

#endif  // USGSTAB_H