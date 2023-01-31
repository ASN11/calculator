#include "deposit.h"

#include "ui_deposit.h"

deposit::deposit(QWidget *parent)
    : QWidget(parent), ui(new Ui::deposit), validator(0, 1000000000, 15, this) {
  ui->setupUi(this);

  // ограничиваем пользовательский ввод
  QLocale locale(QLocale::English);
  validator.setLocale(locale);
  validator.setNotation(QDoubleValidator::StandardNotation);
  ui->display_percent->setValidator(&validator);
  ui->display_sum->setValidator(&validator);
  ui->display_term->setValidator(&validator);

  this->setWindowTitle("Депозитный Калькулятор");

  // устанавливаем сегодняшнюю дату
  ui->dateEdit->setDate(QDate::currentDate());
  ui->dateEdit->setMinimumDate(QDate::currentDate());

  this->createUI(QStringList()
                 << ("Месяц, №") << ("Начисленный %") << ("Сумма итого"));
}

deposit::~deposit() { delete ui; }

void deposit::createUI(const QStringList &headers) {
  ui->table_result->setColumnCount(3);  // Указываем число колонок
  ui->table_result->setShowGrid(true);  // Включаем сетку
  ui->table_result->setStyleSheet("QTableView { gridline-color: #696969; }");
  ui->table_result->setHorizontalHeaderLabels(
      headers);  // Устанавливаем заголовки колонок
  ui->table_result->setSelectionMode(
      QAbstractItemView::SingleSelection);  // Разрешаем выделение только одного
                                            // элемента
  ui->table_result->setSelectionBehavior(
      QAbstractItemView::SelectRows);  // Разрешаем выделение построчно
  ui->table_result->horizontalHeader()->setStretchLastSection(
      true);  // Растягиваем последнюю колонку на всё доступное пространство
}

void deposit::on_button_back_clicked() {
  this->close();       // Закрываем окно
  emit firstWindow();  // И вызываем сигнал на открытие главного окна
}

void deposit::on_button_result_clicked() {
  for (int i = term; i >= 0; i--) ui->table_result->removeRow(i);
  ui->table_result->clearContents();
  double sum = ui->display_sum->text().toDouble();
  term = ui->display_term->text().toInt();
  double percent = ui->display_percent->text().toDouble();
  int type = 0;
  if (ui->radioButton1_fix->isChecked())
    type = 1;
  else if (ui->radioButton1_perc->isChecked())
    type = 2;
  else if (ui->radioButton1_date->isChecked())
    type = 2;
  double type2 = 1;
  if (ui->radioButton2_day->isChecked())
    type2 = 1.02;
  else if (ui->radioButton2_week->isChecked())
    type2 = 1.03;
  else if (ui->radioButton2_month->isChecked())
    type2 = 1.04;
  if (sum && percent && term != 0 && type != 0 && type2 != 1) {
    control.set_credit_calc_data(sum, percent, term, type);
    std::vector<double> payment(control.get_credit_payment());
    std::vector<double> overpayment(control.get_credit_overpayment());
    std::vector<double> out_sum(control.get_credit_out_sum());

    out_sum[0] = sum;

    for (int i = 0; i < term; i++) {
      overpayment[i] = overpayment[i] * type2;
      out_sum[i] = out_sum[i] * type2;
      if (i != 0) out_sum[i] = out_sum[i - 1] + overpayment[i];
      ui->table_result->insertRow(i);  // Вставляем строку
      ui->table_result->setItem(i, 0,
                                new QTableWidgetItem(QString::number(i + 1)));
      ui->table_result->setItem(
          i, 1, new QTableWidgetItem(QString::number(overpayment[i], 'f', 2)));
      ui->table_result->setItem(
          i, 2, new QTableWidgetItem(QString::number(out_sum[i], 'f', 2)));
    }
    ui->table_result
        ->resizeColumnsToContents();  // Ресайзим колонки по содержимому
  } else {
    QMessageBox::critical(this, "ОШИБКА", "Введите данные");
  }
}
