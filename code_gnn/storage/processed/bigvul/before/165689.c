bool SetEnvironmentString16(const std::wstring& variable_name,
                            const std::wstring& new_value) {
  return !!SetEnvironmentVariable(variable_name.c_str(), new_value.c_str());
}
