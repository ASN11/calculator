#include "calc_controller.h"

namespace s21 {

double calc_controller::calculate(std::string string) {
  model.set_string(string);
  model.parse_string();
  model.priority_token();
  model.OPZ();
  model.math();
  return model.get_result();
}

void calc_controller::set_credit_calc_data(double sum, double percent, int term,
                                           int type) {
  credit_model.set_sum(sum);
  credit_model.set_percent(percent);
  credit_model.set_term(term);
  credit_model.set_type(type);
  credit_model.calculate_credit();
}
}  // namespace s21
