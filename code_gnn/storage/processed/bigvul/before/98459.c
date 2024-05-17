void Preferences::SetLanguageConfigStringListAsCSV(const char* section,
                                                   const char* name,
                                                   const std::string& value) {
  LOG(INFO) << "Setting " << name << " to '" << value << "'";

  std::vector<std::string> split_values;
  if (!value.empty())
    SplitString(value, ',', &split_values);

  SetLanguageConfigStringList(section, name, split_values);
}
