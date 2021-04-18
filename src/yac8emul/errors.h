#include <exception>
#include <string>

namespace yac8emul {

namespace errors {

struct error : public std::exception {
  const char *what() const throw() override { return "internal error"; }
  virtual std::string get_info() {
    std::string msg = error::what();
    return msg;
  };
};

struct invalid_instruction : public error {
  std::string kind;
  std::string value;
  std::string msg;

  invalid_instruction(std::string kind_, std::string value_)
      : kind(kind_), value(value_){};
  const char *what() const throw() override { return "invalid instruction"; }

  std::string get_info() override {
    this->msg = "invalid instruction: " + kind + " " + value;
    return this->msg;
  }
};

struct not_implemented : public error {
  std::string instruction;
  std::string msg;

  not_implemented(std::string instruction_) : instruction(instruction_){};
  const char *what() const throw() override { return "not implemented"; }
  std::string get_info() override {
    this->msg = "not implemented: " + instruction;
    return this->msg;
  }
};

} // namespace errors

} // namespace yac8emul
