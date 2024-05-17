std::string GetEnvironmentString(const std::string& variable_name) {
  return UTF16ToUTF8(
      GetEnvironmentString16(UTF8ToUTF16(variable_name).c_str()));
}
