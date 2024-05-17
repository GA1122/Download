bool HasEnvironmentVariable(const std::string& variable_name) {
  return HasEnvironmentVariable16(UTF8ToUTF16(variable_name));
}
