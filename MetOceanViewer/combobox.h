#ifndef COMBOBOX_H
#define COMBOBOX_H

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

class ComboBox : public QWidget {
  Q_OBJECT
 public:
  explicit ComboBox(QWidget *parent = nullptr);
  explicit ComboBox(const QString &label, QWidget *parent = nullptr);

  QComboBox *combo() const;

  QLabel *label() const;

  void setToSizeHint();

 private:
  void initialize(const QString &label = QString());

  QComboBox *m_combo;
  QLabel *m_label;
};

#endif  // COMBOBOX_H