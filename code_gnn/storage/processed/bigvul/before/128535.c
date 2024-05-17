bool Instance::Confirm(const std::string& message) {
  pp::Var result = GetWindowObject().Call("confirm", message);
  return result.is_bool() ? result.AsBool() : false;
}
