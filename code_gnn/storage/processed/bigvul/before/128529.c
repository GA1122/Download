pp::Var PDFScriptableObject::Call(const pp::Var& method,
                                  const std::vector<pp::Var>& args,
                                  pp::Var* exception) {
  return instance_->CallScriptableMethod(method, args, exception);
}
