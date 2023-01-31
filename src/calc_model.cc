#include "calc_model.h"

namespace s21 {
void calc_model::set_string(std::string string) {
  if (string.size() > 255) throw std::out_of_range("array oversize");
  string_ = string;
}

// разделение примера на токены
void calc_model::parse_string() {
  std::vector<std::string> regular = {
      "0-9",  "(",    ")",    "+",   "-",   "*",   "/",    "^",  "mod", "x",
      "acos", "asin", "atan", "cos", "sin", "tan", "sqrt", "ln", "log", " "};
  int token_counter = 0;
  size_t len;
  value_.clear();
  while (string_.size() > 0) {
    int err_flag = token_counter;
    for (size_t i = 0; i < regular.size(); i++) {
      if (i == 0) {
        if (string_.find_first_of(".0123456789") == 0) {
          part_.value_ = "number";
          part_.number_ = std::stod(string_, &len);
          value_.push_back(part_);
          string_.erase(0, len);
          token_counter++;
        }
      } else {
        if (string_.find(regular[i]) == 0) {
          part_.value_ = regular[i];
          part_.number_ = 0;
          value_.push_back(part_);
          string_.erase(0, regular[i].size());
          token_counter++;
        }
      }
    }
    if (err_flag ==
        token_counter) {  // в строке попался какой-то недопустимый символ
      throw std::out_of_range("недопустимый символ");
    }
  }
  search_x();
  check_string();
  unary_minus();
}

// проверка на унарный минус и плюс
void calc_model::unary_minus() {
  //        if (value_[0].value_ == "-" && value_[1].value_ == "(")
  //            throw std::out_of_range("error7");
  if (value_[0].value_ == "-" && value_[1].value_ == "number") {
    value_[1].number_ *= -1;
    value_.erase(value_.begin());
  }
  if (value_[0].value_ == "+") value_.erase(value_.begin());
  for (size_t i = 1; i < value_.size() - 1; i++) {
    if (value_[i].value_ == "-" && value_[i - 1].value_ == "(" &&
        value_[i + 1].value_ == "number") {
      value_[i + 1].number_ *= -1;
      value_.erase(value_.begin() + i);
    } else if (value_[i].value_ == "+" && value_[i - 1].value_ == "(" &&
               value_[i + 1].value_ == "number") {
      value_.erase(value_.begin() + i);
    }
  }
}

// проверяем пример на корректность ввода
void calc_model::check_string() {
  if (value_.size() == 0) throw std::out_of_range("error3");
  //        if (value_.size() == 1 && value_[0].value_ != "number")
  //            throw std::out_of_range("error4");
  //        if (value_.back().value_ != "number" && value_.back().value_ != ")")
  //            throw std::out_of_range("error5");
  //        if (value_.front().value_ == "*" || value_.front().value_ == "/" ||
  //                value_.front().value_ == "mod" || value_.front().value_ ==
  //                "^")
  //            throw std::out_of_range("error6");
  int bracket_count = 0;
  for (size_t i = 0; i < value_.size(); i++) {
    if (value_[i].value_ == "(") bracket_count++;
    if (value_[i].value_ == ")") bracket_count--;
    if (bracket_count < 0) throw std::out_of_range("лишние скобки");
    std::vector<std::string> regular = {"acos", "asin", "atan", "cos", "sin",
                                        "tan",  "sqrt", "ln",   "log"};
    for (int j = 0; j < 9; j++) {
      if (value_[i].value_ == regular[j] && value_[i + 1].value_ != "(")
        throw std::out_of_range("error1");
    }
    std::vector<std::string> regular2 = {"+", "*", "/", "^", "-"};
    for (int j = 0; j < 5; j++) {
      for (int k = 0; k < 5; k++) {
        if (value_[i].value_ == regular2[j] &&
            value_[i + 1].value_ == regular2[k])
          throw std::out_of_range("error2");
      }
    }
    if (value_[i].value_ == "(" && value_[i + 1].value_ == ")")
      throw std::out_of_range("пустые скобки");
  }
  if (bracket_count != 0) throw std::out_of_range("лишние скобки");
}

// назначение приоритета
void calc_model::priority_token() {
  for (size_t i = 0; i < value_.size(); i++) {
    if (value_[i].value_ == "number" || value_[i].value_ == "x")
      value_[i].priority_ = 1;
    else if (value_[i].value_ == "(")
      value_[i].priority_ = -1;
    else if (value_[i].value_ == ")")
      value_[i].priority_ = -2;
    else if (value_[i].value_ == "+" || value_[i].value_ == "-")
      value_[i].priority_ = 2;
    else if (value_[i].value_ == "*" || value_[i].value_ == "/" ||
             value_[i].value_ == "mod")
      value_[i].priority_ = 3;
    else if (value_[i].value_ == " ")
      value_[i].priority_ = 7;
    else
      value_[i].priority_ = 4;
  }
  //        for (int i = 0; i < value_.size(); i++)
  //            std::cout << value_[i].value_ << "\t" << value_[i].number_ <<
  //            "\t" << value_[i].priority_ << std::endl;
}

// сортировочная станция
void calc_model::OPZ() {
  output_.clear();
  std::list<token> stack;
  for (size_t i = 0; i < value_.size(); i++) {
    for (int j = 2; j < 5; j++) {
      if (value_[i].priority_ == j) {  // +, -
        if (stack.empty() || stack.begin()->priority_ < j ||
            (value_[i].value_ == "^" && stack.begin()->value_ == "^")) {
          stack.push_front(value_[i]);
        } else {
          while (!stack.empty() && stack.begin()->priority_ >= j) {
            output_.push_back(*stack.begin());
            stack.pop_front();
          }
          stack.push_front(value_[i]);
        }
      }
    }
    if (value_[i].priority_ == 1) {  // числа, Х
      output_.push_back(value_[i]);
    } else if (value_[i].priority_ == -1) {  // обработка скобок (
      stack.push_front(value_[i]);
    } else if (value_[i].priority_ == -2) {  // обработка скобок )
      while (stack.begin()->priority_ != -1) {
        output_.push_back(*stack.begin());
        stack.pop_front();
      }
      stack.pop_front();
    }
  }
  while (!stack.empty()) {  // закидываем в готовую очередь все, что осталось в
                            // стеке
    output_.push_back(*stack.begin());
    stack.pop_front();
  }
  //        for (int i = 0; i < output_.size(); i++) {
  //            if (output_[i].value_ == "number")
  //                std::cout << output_[i].number_ << " ";
  //            else
  //                std::cout << output_[i].value_ << " ";
  //        }
}

// считаем результат на основе ОПЗ
void calc_model::math() {
  value_.clear();
  std::list<double> stack;
  std::vector<std::string> function = {"cos",  "sin",  "tan",  "acos",
                                       "asin", "atan", "sqrt", "ln",
                                       "log",  "mod",  "^"};
  double (*func[9])(double) = {&cos,  &sin,  &tan, &acos, &asin,
                               &atan, &sqrt, &log, &log10};
  double (*func2[2])(double, double) = {&fmod, &pow};
  if (output_.size() == 1)  // если ввели одно число и больше ничего
    result_ = output_[0].number_;
  for (size_t i = 0; i < output_.size(); i++) {
    if (output_[i].priority_ == 1) {  // если получаем число
      stack.push_front(output_[i].number_);
    } else if (output_[i].priority_ == 2 || output_[i].value_ == "*" ||
               output_[i].value_ == "/") {
      math_plus_minus(stack, i);
    } else {
      for (int j = 0; j < 2; j++) {  // mod, ^
        if (output_[i].value_ == function[j + 9]) {
          if (stack.size() < 2) throw std::out_of_range("error10");
          result_ = func2[j](*++stack.begin(), *stack.begin());
          stack.pop_front();
          stack.pop_front();
          stack.push_front(result_);
        }
      }
      for (int j = 0; j < 9; j++) {  // sin, cos и т.п.
        if (output_[i].value_ == function[j]) {
          result_ = func[j](*stack.begin());
          stack.pop_front();
          stack.push_front(result_);
          break;
        }
      }
    }
  }
  if (stack.size() != 1) throw std::out_of_range("error11");
}

// считаем результат +-*/
void calc_model::math_plus_minus(std::list<double> &stack, size_t i) {
  if (stack.size() < 2) throw std::out_of_range("error10");
  if (output_[i].value_ == "+") {
    result_ = *stack.begin() + *++stack.begin();
    stack.pop_front();
    stack.pop_front();
    stack.push_front(result_);
  } else if (output_[i].value_ == "-") {
    result_ = *++stack.begin() - *stack.begin();
    stack.pop_front();
    stack.pop_front();
    stack.push_front(result_);
  } else if (output_[i].value_ == "/") {
    result_ = *++stack.begin() / *stack.begin();
    stack.pop_front();
    stack.pop_front();
    stack.push_front(result_);
  } else if (output_[i].value_ == "*") {
    result_ = *stack.begin() * *++stack.begin();
    stack.pop_front();
    stack.pop_front();
    stack.push_front(result_);
  }
}

// заменяем х на число
void calc_model::search_x() {
  for (size_t i = 0; i < value_.size(); i++) {
    if (value_[i].value_ == "x") {
      value_[i].number_ = x_;
      value_[i].value_ = "number";
    }
  }
}
}  // namespace s21
