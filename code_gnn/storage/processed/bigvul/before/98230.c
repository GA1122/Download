  virtual bool SetVar(const char* variable_name, const std::string& new_value) {
    return SetVarImpl(variable_name, new_value);
  }
