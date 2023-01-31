#include "deposit_calc.h"

namespace s21 {
void deposit_calc::calculate_credit() {
  payment_.clear();
  overpayment_.clear();
  out_sum_.clear();
  percent_ = percent_ / 100. / 12.;
  if (type_ == 1) {
    double ann_payment =
        sum_ * (percent_ / (1 - powl(1 + percent_, -1 * term_)));
    ann_payment = round(ann_payment * 100) / 100.;
    for (int i = 0; i < term_; i++) {
      payment_.push_back(ann_payment);
      out_sum_.push_back(payment_[i] * (i + 1));
      overpayment_.push_back(out_sum_[i] - sum_ / term_ * (i + 1));
    }
  } else if (type_ == 2) {
    double local_sum = sum_ / (double)term_;
    double sum_static = sum_;
    for (int i = 0; i < term_; i++) {
      payment_.push_back(round((local_sum + sum_ * percent_) * 100) / 100.);
      if (i == 0)
        out_sum_.push_back(payment_[i]);
      else
        out_sum_.push_back(out_sum_[i - 1] + payment_[i]);
      overpayment_.push_back(out_sum_[i] - sum_static / term_ * (i + 1));
      sum_ -= local_sum;
    }
  }
}
}  // namespace s21
