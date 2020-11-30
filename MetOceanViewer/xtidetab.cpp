#include "xtidetab.h"

#include <memory>

#include "generic.h"
#include "xtidedata.h"

XTideTab::XTideTab(std::vector<MovStation> *stations, QWidget *parent)
    : MapChartWidget(TabType::XTIDE, stations, parent), m_cbx_datum(nullptr),
      m_cbx_units(nullptr), m_btn_compute(nullptr) {
  this->initialize();
}

void XTideTab::connectSignals() {
  connect(this->m_btn_compute, SIGNAL(clicked()), this, SLOT(plot()));
  MapChartWidget::connectSignals();
}

void XTideTab::refreshStations() {
  this->mapWidget()->refreshStations(true, false);
}

QGroupBox *XTideTab::generateInputBox() {
  auto *input = new QGroupBox(this);
  input->setTitle("Select a station to compute tides");

  this->setStartDateEdit(new DateBox("Start Date:", this));
  this->setEndDateEdit(new DateBox("End Date:", this));
  this->setChartOptions(
      new ChartOptionsMenu(true, true, true, false, true, true, this));
  this->m_cbx_datum = new ComboBox("Datum:", this);
  this->m_cbx_units = new ComboBox("Units:", this);
  this->m_btn_compute = new QPushButton("Compute Tides", this);
  this->setCbx_mapType(new ComboBox("Map:", this));

  QStringList dList;
  for (auto &i : Datum::vDatumList()) {
    dList << QString::fromStdString(std::string(i));
  }

  this->m_cbx_datum->combo()->addItems(dList);
  this->m_cbx_units->combo()->addItems(QStringList() << "metric"
                                                     << "english");

  this->mapWidget()->mapFunctions()->setMapTypes(this->cbx_mapType()->combo());

  QTime tm(0, 0, 0);
  QDateTime startDate = QDateTime::currentDateTimeUtc().addDays(-7);
  QDateTime endDate = QDateTime::currentDateTimeUtc().addDays(1);
  startDate.setTime(tm);
  endDate.setTime(tm);
  this->startDateEdit()->dateEdit()->setDateTime(startDate);
  this->endDateEdit()->dateEdit()->setDateTime(endDate);

  this->startDateEdit()->dateEdit()->setDisplayFormat("MM/dd/yyyy");
  this->endDateEdit()->dateEdit()->setDisplayFormat("MM/dd/yyyy");

  auto *r1 = new QHBoxLayout();
  auto *r2 = new QHBoxLayout();
  auto *r3 = new QHBoxLayout();

  r1->addWidget(this->startDateEdit());
  r1->addWidget(this->endDateEdit());
  r2->addWidget(this->m_cbx_datum);
  r2->addWidget(this->m_cbx_units);
  r2->addWidget(this->m_btn_compute);
  r3->addWidget(this->cbx_mapType());
  r3->addWidget(this->chartOptions());
  r1->addStretch();
  r2->addStretch();
  r3->addStretch();

  auto *v = new QVBoxLayout();
  v->addLayout(r1);
  v->addLayout(r2);
  v->addLayout(r3);
  v->addStretch();

  input->setLayout(v);
  input->setMinimumHeight(input->minimumSizeHint().height());
  input->setMaximumHeight(input->minimumSizeHint().height() + 10);
  return input;
}

void XTideTab::plot() {
  this->chartview()->clear();
  this->chartview()->initializeAxis();
  MovStation s = this->mapWidget()->currentStation();
  if (s.id() == "null") {
    emit error("No station was selected");
    return;
  }
  qint64 tzOffset;
  QDateTime start, end, startgmt, endgmt;
  QString tzAbbrev;
  int ierr =
      this->calculateDateInfo(start, end, startgmt, endgmt, tzAbbrev, tzOffset);
  if (ierr != 0)
    return;

  Datum::VDatum datum =
      Datum::datumID(this->m_cbx_datum->combo()->currentText().toStdString());
  this->data()->reset(new Hmdf::HmdfData());

  auto xtide = std::make_unique<XtideData>(s,start,end,Generic::configDirectory());
  ierr = xtide->get(this->data()->get(), datum);
  if (ierr != 0) {
    emit error(QString::fromStdString(xtide->errorString()));
    return;
  }

  QString unitString = "m";
  if (this->m_cbx_units->combo()->currentText() == "english") {
    unitString = "ft";
    for (auto t : *this->data()->get()->station(0)) {
      t.set(t.date(), t.value() * 3.28084);
    }
  }

  this->setPlotAxis(this->data()->get(), start, end, tzAbbrev,
                    QString::fromStdString(Datum::datumName(datum)), unitString,
                    "Water Level");
  this->addSeriesToChart(this->data()->get(), tzOffset);
  this->chartview()->initializeAxisLimits();
  this->chartview()->initializeLegendMarkers();
  this->chartview()->chart()->setTitle("XTide: " + s.name());
}

void XTideTab::addSeriesToChart(Hmdf::HmdfData *data, const qint64 tzOffset) {
  QLineSeries *series =
      this->stationToSeries(this->data()->get()->station(0), tzOffset);
  series->setName(QString::fromStdString(data->station(0)->name()));
  series->setPen(
      QPen(QColor(Qt::green), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  this->chartview()->addSeries(
      series, QString::fromStdString(data->station(0)->name()));
}