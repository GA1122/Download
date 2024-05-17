bool OutOfProcessInstance::Confirm(const std::string& message) {
  pp::Var result = ModalDialog(this, "confirm", message, std::string());
  return result.is_bool() ? result.AsBool() : false;
}
