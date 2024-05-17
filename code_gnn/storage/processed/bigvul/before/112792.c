void PrintPreviewUI::OnInvalidPrinterSettings() {
  web_ui()->CallJavascriptFunction("invalidPrinterSettings");
}
