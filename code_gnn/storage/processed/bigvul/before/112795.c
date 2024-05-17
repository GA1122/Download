void PrintPreviewUI::OnReloadPrintersList() {
  web_ui()->CallJavascriptFunction("reloadPrintersList");
}
