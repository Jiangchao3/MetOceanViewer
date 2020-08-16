#ifndef USERDATATABLE_H
#define USERDATATABLE_H

#include <QCheckBox>
#include <QDateEdit>
#include <QDateTime>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QString>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

#include "textentry.h"

class UserdataTable : public QWidget {
  Q_OBJECT
 public:
  explicit UserdataTable(QWidget *parent = nullptr);

 private slots:
  void onCheckDateAxisAuto(bool checked);
  void onCheckYAxisAuto(bool checked);
  void onClickAddSeries();
  void onClickEditSeries();
  void onClickDeleteSeries();
  void onClickCopySeries();
  void onClickMoveUpSeries();
  void onClickMoveDownSeries();

 private:
  QTableWidget *m_tableWidget;

  QPushButton *m_addButton;
  QPushButton *m_editButton;
  QPushButton *m_copyButton;
  QPushButton *m_deleteButton;
  QPushButton *m_upButton;
  QPushButton *m_downButton;

  QCheckBox *m_dateCheck;
  QCheckBox *m_yCheck;

  QDateEdit *m_startDate;
  QDateEdit *m_endDate;

  QDoubleSpinBox *m_ymin;
  QDoubleSpinBox *m_ymax;

  TextEntry<QString> *m_title;
  TextEntry<QString> *m_xlabel;
  TextEntry<QString> *m_ylabel;

  QHBoxLayout *generateButtonBar();
  QHBoxLayout *generateAxisOptions();
  QGroupBox *generateLabelOptions();

  struct SeriesObject {

  };

  QVector<SeriesObject> m_seriesList;
};

#endif  // USERDATATABLE_H
