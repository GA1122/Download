bool SetEnvironmentString(const std::string& variable_name,
                          const std::string& new_value) {
  return SetEnvironmentString16(UTF8ToUTF16(variable_name),
                                UTF8ToUTF16(new_value));
}
