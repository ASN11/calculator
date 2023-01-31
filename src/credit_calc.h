#ifndef CREDIT_CALC_H
#define CREDIT_CALC_H

#include <cmath>
#include <vector>

namespace s21 {
class credit_calc {
 private:
  double sum_, percent_;
  int term_, type_;
  std::vector<double> payment_, overpayment_, out_sum_;

 public:
  void set_sum(double sum) { sum_ = sum; }
  void set_percent(double percent) { percent_ = percent; }
  void set_term(int term) { term_ = term; }
  void set_type(int type) { type_ = type; }
  void calculate_credit();  // само вычисление
  std::vector<double> get_payment() { return payment_; }
  std::vector<double> get_overpayment() { return overpayment_; }
  std::vector<double> get_out_sum() { return out_sum_; }
};
}  // namespace s21

#endif  // CREDIT_CALC_H
