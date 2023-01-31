#ifndef CALC_CONTROLLER_H
#define CALC_CONTROLLER_H

#include "calc_model.h"
#include "credit_calc.h"
#include "deposit_calc.h"

namespace s21 {
class calc_controller {
 private:
  calc_model model;
  credit_calc credit_model;

 public:
  double calculate(std::string string);
  void set_x(double value) { model.set_x(value); }
  void set_credit_calc_data(double sum, double percent, int term, int type);
  std::vector<double> get_credit_payment() {
    return credit_model.get_payment();
  }
  std::vector<double> get_credit_overpayment() {
    return credit_model.get_overpayment();
  }
  std::vector<double> get_credit_out_sum() {
    return credit_model.get_out_sum();
  }
};
}  // namespace s21

#endif  // CALC_CONTROLLER_H
