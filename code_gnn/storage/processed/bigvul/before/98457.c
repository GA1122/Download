void Preferences::SetLanguageConfigString(const char* section,
                                          const char* name,
                                          const std::string& value) {
  ImeConfigValue config;
  config.type = ImeConfigValue::kValueTypeString;
  config.string_value = value;
  CrosLibrary::Get()->GetInputMethodLibrary()->
      SetImeConfig(section, name, config);
}
