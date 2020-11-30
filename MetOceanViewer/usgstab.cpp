#include "usgstab.h"

#include <memory>

#include "metocean.h"
#include "usgswaterdata.h"

UsgsTab::UsgsTab(std::vector<MovStation> *stations, QWidget *parent)
    : MapChartWidget(TabType::USGS, stations, parent), m_ready(false),
      m_btn_fetch(nullptr), m_buttonGroup(nullptr), m_lbl_buttonGroup(nullptr),
      m_cbx_product(nullptr), m_rbtn_daily(nullptr), m_rbtn_instant(nullptr) {
  this->initialize();
}

void UsgsTab::connectSignals() {
  connect(this->m_btn_fetch, SIGNAL(clicked()), this, SLOT(plot()));
  connect(this->m_cbx_product->combo(), SIGNAL(currentIndexChanged(int)), this,
          SLOT(replot(int)));
  MapChartWidget::connectSignals();
}

QGroupBox *UsgsTab::generateInputBox() {
  auto *input = new QGroupBox(this);
  input->setTitle(
      "Select a station and click fetch to generate list of available "
      "products");
  auto *v = new QVBoxLayout();

  this->setStartDateEdit(new DateBox("Start Date:", this));
  this->setEndDateEdit(new DateBox("End Date:", this));
  this->setTimezoneCombo(new ComboBox("Time zone:", this));
  this->m_cbx_product = new ComboBox("Product:", this);
  this->setCbx_mapType(new ComboBox("Map:", this));
  this->m_btn_fetch = new QPushButton("Fetch Data", this);

  this->m_lbl_buttonGroup = new QLabel("Data Type:", this);
  this->m_rbtn_instant = new QRadioButton("Instant Data", this);
  this->m_rbtn_daily = new QRadioButton("Daily Data", this);

  this->m_buttonGroup = new QButtonGroup(this);
  this->m_buttonGroup->addButton(this->m_rbtn_instant, 1);
  this->m_buttonGroup->addButton(this->m_rbtn_daily, 2);

  this->m_rbtn_instant->setChecked(true);

  this->setChartOptions(
      new ChartOptionsMenu(true, true, true, false, true, true, this));

  QDateTime startDate = QDateTime::currentDateTimeUtc().addDays(-7);
  QDateTime endDate = QDateTime::currentDateTimeUtc();

  this->startDateEdit()->dateEdit()->setDateTime(startDate);
  this->endDateEdit()->dateEdit()->setDateTime(endDate);

  this->m_rowLayouts.resize(4);
  for (auto &r : this->m_rowLayouts) {
    r = new QHBoxLayout();
  }

  this->timezoneCombo()->combo()->addItems(timezoneList());
  this->mapWidget()->mapFunctions()->setMapTypes(this->cbx_mapType()->combo());

  this->timezoneCombo()->combo()->setCurrentText("GMT");
  this->m_cbx_product->combo()->setMinimumWidth(350);

  this->m_rowLayouts[0]->addWidget(this->startDateEdit());
  this->m_rowLayouts[0]->addWidget(this->endDateEdit());
  this->m_rowLayouts[0]->addSpacerItem(
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Preferred));
  this->m_rowLayouts[1]->addWidget(this->timezoneCombo());
  this->m_rowLayouts[1]->addWidget(this->cbx_mapType());
  this->m_rowLayouts[1]->addSpacerItem(
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Preferred));
  this->m_rowLayouts[2]->addWidget(this->m_lbl_buttonGroup);
  this->m_rowLayouts[2]->addWidget(this->m_rbtn_instant);
  this->m_rowLayouts[2]->addWidget(this->m_rbtn_daily);
  this->m_rowLayouts[2]->addWidget(this->m_btn_fetch);
  this->m_rowLayouts[2]->addSpacerItem(
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Preferred));
  this->m_rowLayouts[3]->addWidget(this->m_cbx_product);
  this->m_rowLayouts[3]->addWidget(this->chartOptions());
  this->m_rowLayouts[3]->addSpacerItem(
      new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Preferred));

  for (auto &r : this->m_rowLayouts) {
    r->addStretch();
    v->addLayout(r);
  }

  v->addStretch();
  input->setLayout(v);
  input->setMinimumHeight(input->minimumSizeHint().height());
  input->setMaximumHeight(input->minimumSizeHint().height() + 10);

  return input;
}

int UsgsTab::getDatabaseType() { return this->m_buttonGroup->checkedId(); }

void UsgsTab::plot() {
  this->m_ready = false;
  this->m_currentStation = this->mapWidget()->currentStation();
  if (this->m_currentStation.id() == "null") {
    emit error("No station was selected");
    return;
  }

  int type = this->getDatabaseType();
  *this->data() = std::make_unique<Hmdf::HmdfData>();
  std::unique_ptr<UsgsWaterdata> usgs(new UsgsWaterdata(
      this->m_currentStation, this->startDateEdit()->dateEdit()->dateTime(),
      this->endDateEdit()->dateEdit()->dateTime(), type));
  int ierr = usgs->get(this->data()->get());
  if (ierr != 0) {
    emit error(QString::fromStdString(usgs->errorString()));
    return;
  }

  this->m_cbx_product->combo()->clear();
  for (size_t i = 0; i < this->data()->get()->nStations(); ++i) {
    this->m_cbx_product->combo()->addItem(
        QString::fromStdString(this->data()->get()->station(i)->name()));
  }

  this->m_ready = true;
  this->replot(0);
}

void UsgsTab::replot(int index) {
  if (this->data() && this->m_ready) {
    this->chartview()->clear();
    this->chartview()->initializeAxis();

    qint64 tzOffset;
    QDateTime start, end, startgmt, endgmt;

    QString tzAbbrev;
    int ierr = this->calculateDateInfo(start, end, startgmt, endgmt, tzAbbrev,
                                       tzOffset);
    if (ierr != 0)
      return;
    QString unitString =
        QString::fromStdString(data()->get()->station(index)->units());
    QString productName = UsgsTab::splitUsgsProductName(
        this->data()->get()->station(index)->name());
    this->setPlotAxis(this->data()->get(), start, end, tzAbbrev, QString(),
                      unitString, productName, index);
    this->chartview()->chart()->setTitle(
        QString::fromStdString(this->m_currentStation.name().toStdString()));
    this->addSeriesToChart(
        index,
        "USGS" +
            QString::fromStdString(this->m_currentStation.id().toStdString()),
        tzOffset);
    this->chartview()->initializeAxisLimits();
    this->chartview()->initializeLegendMarkers();
  }
}

void UsgsTab::addSeriesToChart(const int index, const QString &name,
                               const qint64 tzOffset) {
  QLineSeries *series =
      this->stationToSeries(this->data()->get()->station(index), tzOffset);
  series->setName(name);
  series->setPen(
      QPen(QColor(Qt::blue), 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
  this->chartview()->addSeries(series, name);
}

QString UsgsTab::splitUsgsProductName(const std::string &product) {
  return QString::fromStdString(product.substr(0, product.rfind(',')));
}

void UsgsTab::saveData() {
  Hmdf::HmdfData data;
  data.station(0)->setDatum(this->data()->get()->station(0)->datum());
  data.station(0)->setUnits(this->data()->get()->station(0)->units());
  data.addStation(*(this->data()->get()->station(
      this->m_cbx_product->combo()->currentIndex())));
  this->writeData(&data);
}