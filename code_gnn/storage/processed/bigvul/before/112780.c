void PrintPreviewHandler::SendPrinterCapabilities(
    const DictionaryValue& settings_info) {
  VLOG(1) << "Get printer capabilities finished";
  scoped_ptr<DictionaryValue> settings(settings_info.DeepCopy());
  if (GetStickySettings()->color_model() != printing::UNKNOWN_COLOR_MODEL) {
    settings->SetBoolean(
        printing::kSettingSetColorAsDefault,
        printing::isColorModelSelected(
            GetStickySettings()->color_model()));
  }
  web_ui()->CallJavascriptFunction("updateWithPrinterCapabilities",
                                   *settings);
}
