bool HasEnvironmentVariable16(const std::wstring& variable_name) {
  return !!::GetEnvironmentVariable(variable_name.c_str(), nullptr, 0);
}
