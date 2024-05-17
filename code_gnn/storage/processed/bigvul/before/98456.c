void Preferences::SetLanguageConfigInteger(const char* section,
                                           const char* name,
                                           int value) {
  ImeConfigValue config;
  config.type = ImeConfigValue::kValueTypeInt;
  config.int_value = value;
  CrosLibrary::Get()->GetInputMethodLibrary()->
      SetImeConfig(section, name, config);
}
