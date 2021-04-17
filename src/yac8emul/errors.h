#include <exception>
#include <string>

namespace yac8emul {

namespace errors {
struct invalid_instruction : public std::exception {
  std::string kind;
  std::string value;
  std::string msg;

  invalid_instruction(std::string kind_, std::string value_)
      : kind(kind_), value(value_){};
  const char *what() const throw() { return "invalid instruction"; }

  const char *get_info() {
    this->msg = "invalid instruction: " + kind + " " + value;
    return this->msg.c_str();
  }
};

struct not_implemented : public std::exception {
  std::string instruction;
  std::string msg;

  not_implemented(std::string instruction_) : instruction(instruction_){};
  const char *what() const throw() { return "not implemented"; }
  const char *get_info() {
    this->msg = "not implemented: " + instruction;
    return this->msg.c_str();
  }
};

} // namespace errors

} // namespace yac8emul