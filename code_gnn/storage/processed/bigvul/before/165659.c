std::wstring GetEnvironmentString16(const wchar_t* variable_name) {
  DWORD value_length = ::GetEnvironmentVariableW(variable_name, nullptr, 0);
  if (!value_length)
    return std::wstring();
  std::wstring value(value_length, L'\0');
  value_length =
      ::GetEnvironmentVariableW(variable_name, &value[0], value_length);
  if (!value_length || value_length >= value.size())
    return std::wstring();
  value.resize(value_length);
  return value;
}
