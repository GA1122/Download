  virtual bool GetVar(const char* variable_name, std::string* result) {
    if (GetVarImpl(variable_name, result))
      return true;

    char first_char = variable_name[0];
    std::string alternate_case_var;
    if (first_char >= 'a' && first_char <= 'z')
      alternate_case_var = StringToUpperASCII(std::string(variable_name));
    else if (first_char >= 'A' && first_char <= 'Z')
      alternate_case_var = StringToLowerASCII(std::string(variable_name));
    else
      return false;
    return GetVarImpl(alternate_case_var.c_str(), result);
  }
