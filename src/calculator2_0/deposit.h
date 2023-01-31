#ifndef DEPOSIT_H
#define DEPOSIT_H

#include <QDoubleValidator>
#include <QMessageBox>
#include <QWidget>

#include "../calc_controller.h"

namespace Ui {
class deposit;
}

class deposit : public QWidget {
  Q_OBJECT

 public:
  explicit deposit(QWidget *parent = nullptr);
  ~deposit();

 signals:
  void firstWindow();  // Сигнал для первого окна на открытие

 private slots:
  void on_button_back_clicked();
  void on_button_result_clicked();

 private:
  Ui::deposit *ui;
  QDoubleValidator validator;
  s21::calc_controller control;
  void createUI(const QStringList &headers);
  int term = 0;
};

#endif  // DEPOSIT_H
