std::string OutOfProcessInstance::Prompt(const std::string& question,
                                         const std::string& default_answer) {
  pp::Var result = ModalDialog(this, "prompt", question, default_answer);
  return result.is_string() ? result.AsString() : std::string();
}
