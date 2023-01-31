#ifndef CREDIT_H
#define CREDIT_H

#include <QDoubleValidator>
#include <QMessageBox>
#include <QWidget>

#include "../calc_controller.h"

namespace Ui {
class credit;
}

class credit : public QWidget {
  Q_OBJECT

 public:
  explicit credit(QWidget *parent = nullptr);
  ~credit();

 signals:
  void firstWindow();  // Сигнал для первого окна на открытие

 private slots:
  void on_button_back_clicked();
  void on_button_result_clicked();

 private:
  Ui::credit *ui;
  void createUI(const QStringList &headers);
  int term = 0;
  QDoubleValidator validator;
  s21::calc_controller control;
};

#endif  // CREDIT_H
