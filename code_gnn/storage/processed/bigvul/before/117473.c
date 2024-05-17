base::Value* GetProxySetting(Browser* browser,
                             const std::string& setting_name) {
  std::string setting_path = "cros.session.proxy.";
  setting_path.append(setting_name);

  if (setting_name == "ignorelist") {
    base::Value* value;
    if (chromeos::proxy_cros_settings_parser::GetProxyPrefValue(
            browser->profile(), setting_path, &value)) {
       return value;
    }
  } else {
    base::Value* setting;
    if (chromeos::proxy_cros_settings_parser::GetProxyPrefValue(
            browser->profile(), setting_path, &setting)) {
      DictionaryValue* setting_dict = static_cast<DictionaryValue*>(setting);
      base::Value* value;
      bool found = setting_dict->Remove("value", &value);
      delete setting;
      if (found)
        return value;
    }
  }
  return NULL;
}
