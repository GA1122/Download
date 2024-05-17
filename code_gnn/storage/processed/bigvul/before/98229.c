bool Environment::HasVar(const char* variable_name) {
  return GetVar(variable_name, NULL);
}
