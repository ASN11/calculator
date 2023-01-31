#include "mainwindow.h"

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      validator(-100000, 100000, 15, this),
      validator_x(-1.7E+308, 1.7E+308, 15, this) {
  ui->setupUi(this);
  this->setWindowTitle("Крутой Калькулятор");

  // объявляем соседние окна
  creditWindow = new credit();
  depositWindow = new deposit();
  connect(creditWindow, &credit::firstWindow, this, &MainWindow::show);
  connect(depositWindow, &deposit::firstWindow, this, &MainWindow::show);

  // ограничиваем пользовательский ввод для оси икс
  validator.setNotation(QDoubleValidator::StandardNotation);
  ui->display_x_begin->setValidator(&validator);
  ui->display_x_end->setValidator(&validator);
  ui->display_x_calc->setValidator(&validator_x);

  // отрисовка градиента графика функции
  QLinearGradient plotGradient;
  plotGradient.setStart(0, 0);
  plotGradient.setFinalStop(0, 350);
  plotGradient.setColorAt(0, QColor(80, 80, 80));
  plotGradient.setColorAt(1, QColor(50, 50, 50));
  ui->graphic->setBackground(plotGradient);
  QLinearGradient axisRectGradient;
  axisRectGradient.setStart(0, 0);
  axisRectGradient.setFinalStop(0, 350);
  axisRectGradient.setColorAt(0, QColor(80, 80, 80));
  axisRectGradient.setColorAt(1, QColor(30, 30, 30));
  ui->graphic->setBackground(axisRectGradient);
  // цвет шкалы x и y
  ui->graphic->yAxis->setTickLabelColor(QColorConstants::Svg::silver);
  ui->graphic->xAxis->setTickLabelColor(QColorConstants::Svg::silver);

  connect(ui->button_0, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->button_1, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->button_2, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->button_3, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->button_4, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->button_5, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->button_6, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->button_7, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->button_8, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->button_9, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->button_plus, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->button_minus, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->button_stepen, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->button_tochka, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->button_skobka_1, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->button_skobka_2, SIGNAL(clicked()), this, SLOT(digits_numbers()));
  connect(ui->button_mul, SIGNAL(clicked()), this, SLOT(digits_other()));
  connect(ui->button_div, SIGNAL(clicked()), this, SLOT(digits_other()));
  connect(ui->button_mod, SIGNAL(clicked()), this, SLOT(digits_other()));
  connect(ui->button_sin, SIGNAL(clicked()), this, SLOT(sin_cos()));
  connect(ui->button_cos, SIGNAL(clicked()), this, SLOT(sin_cos()));
  connect(ui->button_tan, SIGNAL(clicked()), this, SLOT(sin_cos()));
  connect(ui->button_asin, SIGNAL(clicked()), this, SLOT(sin_cos()));
  connect(ui->button_acos, SIGNAL(clicked()), this, SLOT(sin_cos()));
  connect(ui->button_atan, SIGNAL(clicked()), this, SLOT(sin_cos()));
  connect(ui->button_sqrt, SIGNAL(clicked()), this, SLOT(sin_cos()));
  connect(ui->button_ln, SIGNAL(clicked()), this, SLOT(sin_cos()));
  connect(ui->button_log, SIGNAL(clicked()), this, SLOT(sin_cos()));
  connect(ui->display, SIGNAL(returnPressed()), SLOT(EnterPressed()));
}

MainWindow::~MainWindow() { delete ui; }

// обработка цифр 0-9, +,-,^,()

void MainWindow::digits_numbers() {
  QPushButton *button = (QPushButton *)sender();
  QString new_label = (ui->display->text() + button->text());
  ui->display->setText(new_label);
}

// обработка *,/,mod

void MainWindow::digits_other() {
  QPushButton *button = (QPushButton *)sender();
  QString new_string = NULL;
  if (button->text() == "x") new_string = (ui->display->text() + "*");
  if (button->text() == "÷") new_string = (ui->display->text() + "/");
  if (button->text() == "%") new_string = (ui->display->text() + "mod");
  ui->display->setText(new_string);
}

// обработка sin, cos и др.

void MainWindow::sin_cos() {
  QPushButton *button = (QPushButton *)sender();
  QString new_label = (ui->display->text() + button->text() + "(");
  ui->display->setText(new_label);
}

// обработка знака равно

void MainWindow::on_button_enter_clicked() {
  if (ui->display->text().length() < 256) {
    QString qstr = ui->display->text();
    std::string str = qstr.toStdString();
    try {
      control.set_x(ui->display_x_calc->text().toDouble());
      double result = control.calculate(str);
      QString valueAsString = QString::number(result, 'g', 15);
      ui->display->setText(valueAsString);
    } catch (...) {
      QMessageBox::critical(this, "ОШИБКА", "Ошибка, некорректный ввод");
      ui->display->setText("");
    }
  } else {
    QMessageBox::critical(this, "ОШИБКА",
                          "Ошибка, превышение длины выражения (255)");
    ui->display->setText("");
  }
}

// обработка AC

void MainWindow::on_button_AC_clicked() { ui->display->setText(""); }

// обработка "<<"

void MainWindow::on_button_back_clicked() {
  QString new_text =
      ui->display->text().left(std::size(ui->display->text()) - 1);
  ui->display->setText(new_text);
}

// обработка нажатия на Enter

void MainWindow::EnterPressed() { MainWindow::on_button_enter_clicked(); }

// отрисовка графика функции

void MainWindow::on_button_graph_clicked() {
  ui->graphic->clearGraphs();
  ui->graphic->setInteraction(QCP::iRangeZoom, true);
  ui->graphic->setInteraction(QCP::iRangeDrag, true);
  ui->graphic->xAxis->setTickLabelFont(QFont(QFont().family(), 8));
  ui->graphic->yAxis->setTickLabelFont(QFont(QFont().family(), 8));

  QString qstr = ui->display->text();
  std::string str = qstr.toStdString();

  double x_begin = ui->display_x_begin->text().toDouble();
  double x_end = ui->display_x_end->text().toDouble();

  try {
    for (X = x_begin; X < x_end; X += 0.1) {
      x.push_back(X);
      control.set_x(X);
      double result = control.calculate(str);
      y.push_back(result);
    }
    ui->graphic->addGraph();
    ui->graphic->graph(0)->addData(x, y);
    ui->graphic->graph(0)->setPen(QPen(Qt::red));
    QCPScatterStyle myScatter;
    myScatter.setShape(QCPScatterStyle::ssCircle);
    myScatter.setPen(QPen(Qt::blue));
    myScatter.setBrush(Qt::white);
    myScatter.setSize(6);
    ui->graphic->graph(0)->setScatterStyle(myScatter);

    ui->graphic->rescaleAxes();
    ui->graphic->replot();

    x.clear();
    y.clear();
  } catch (...) {
    QMessageBox::critical(this, "ОШИБКА", "Ошибка, некорректный ввод");
    ui->display->setText("");
  }
}

// обработка кнопки X

void MainWindow::on_button_X_clicked() {
  QPushButton *button = (QPushButton *)sender();
  QString new_string = NULL;
  if (button->text() == "X") new_string = (ui->display->text() + "x");
  ui->display->setText(new_string);
}

void MainWindow::on_button_credit_clicked() {
  creditWindow->show();  // Показываем второе окно
  this->close();         // Закрываем основное окно
}

void MainWindow::on_button_deposit_clicked() {
  depositWindow->show();  // Показываем второе окно
  this->close();          // Закрываем основное окно
}
