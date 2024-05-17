void AppLauncherHandler::OnExtensionPreferenceChanged() {
  base::DictionaryValue dictionary;
  FillAppDictionary(&dictionary);
  web_ui()->CallJavascriptFunction("ntp.appsPrefChangeCallback", dictionary);
}
