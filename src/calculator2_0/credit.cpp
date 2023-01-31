#include "credit.h"

#include "ui_credit.h"

credit::credit(QWidget *parent)
    : QWidget(parent), ui(new Ui::credit), validator(0, 1000000000, 15, this) {
  ui->setupUi(this);

  // ограничиваем пользовательский ввод
  QLocale locale(QLocale::English);
  validator.setLocale(locale);
  validator.setNotation(QDoubleValidator::StandardNotation);
  ui->display_percent->setValidator(&validator);
  ui->display_sum->setValidator(&validator);
  ui->display_term->setValidator(&validator);

  this->setWindowTitle("Кредитный Калькулятор");

  this->createUI(QStringList() << ("Месяц, №") << ("Платеж") << ("Переплата")
                               << ("Общая сумма"));
}

credit::~credit() { delete ui; }

void credit::createUI(const QStringList &headers) {
  ui->table_result->setColumnCount(4);  // Указываем число колонок
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

void credit::on_button_back_clicked() {
  this->close();       // Закрываем окно
  emit firstWindow();  // И вызываем сигнал на открытие главного окна
}

void credit::on_button_result_clicked() {
  for (int i = term; i >= 0; i--) ui->table_result->removeRow(i);
  ui->table_result->clearContents();
  double sum = ui->display_sum->text().toDouble();
  term = ui->display_term->text().toInt();
  double percent = ui->display_percent->text().toDouble();
  int type = 0;
  if (ui->radioButton_annuitet->isChecked())
    type = 1;
  else if (ui->radioButton_diff->isChecked())
    type = 2;
  if (sum && percent && term != 0 && type != 0) {
    control.set_credit_calc_data(sum, percent, term, type);
    std::vector<double> payment(control.get_credit_payment());
    std::vector<double> overpayment(control.get_credit_overpayment());
    std::vector<double> out_sum(control.get_credit_out_sum());

    for (int i = 0; i < term; i++) {
      ui->table_result->insertRow(i);  // Вставляем строку
      ui->table_result->setItem(i, 0,
                                new QTableWidgetItem(QString::number(i + 1)));
      ui->table_result->setItem(
          i, 1, new QTableWidgetItem(QString::number(payment[i], 'f', 2)));
      ui->table_result->setItem(
          i, 2, new QTableWidgetItem(QString::number(overpayment[i], 'f', 2)));
      ui->table_result->setItem(
          i, 3, new QTableWidgetItem(QString::number(out_sum[i], 'f', 2)));
    }
    ui->table_result
        ->resizeColumnsToContents();  // Ресайзим колонки по содержимому
  } else {
    QMessageBox::critical(this, "ОШИБКА", "Введите данные");
  }
}
