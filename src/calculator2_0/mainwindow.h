#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDoubleValidator>
#include <QKeyEvent>
#include <QMainWindow>
#include <QMessageBox>
#include <QVector>

#include "../calc_controller.h"
#include "credit.h"
#include "deposit.h"
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;
  double X;
  QVector<double> x, y;
  QDoubleValidator validator;
  QDoubleValidator validator_x;
  s21::calc_controller control;
  credit *creditWindow;
  deposit *depositWindow;

 private slots:
  void digits_numbers();
  void digits_other();
  void sin_cos();
  void on_button_enter_clicked();
  void on_button_AC_clicked();
  void on_button_back_clicked();
  void EnterPressed();
  void on_button_X_clicked();
  void on_button_graph_clicked();
  void on_button_credit_clicked();
  void on_button_deposit_clicked();
};
#endif  // MAINWINDOW_H
