#ifndef CALC_MODEL_H
#define CALC_MODEL_H

#include <cmath>
#include <iostream>
#include <list>
#include <string>
#include <vector>

namespace s21 {
class calc_model;
class token {
  friend calc_model;

 private:
  std::string value_;
  double number_ = 0;
  int priority_ = 0;
};

class calc_model {
 private:
  std::string string_;
  std::vector<token> value_;
  std::vector<token> output_;
  token part_;
  double result_ = 0;
  double x_ = 0;

 public:
  void set_string(std::string string);
  void parse_string();  // разделение примера на токены
  void check_string();  // проверяем пример на корректность ввода
  void unary_minus();  // проверка на унарный минус и плюс
  void priority_token();  // назначение приоритета
  void OPZ();             // сортировочная станция
  void math();  // считаем результат на основе ОПЗ
  void math_plus_minus(std::list<double> &stack,
                       size_t i);  // считаем результат +-*/
  void search_x();                 // заменяем х на число
  double get_result() const { return result_; }
  void set_x(double value) { x_ = value; }
};
}  // namespace s21
#endif  // CALC_MODEL_H
