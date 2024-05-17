void PrintPreviewHandler::SetupPrinterList(const ListValue& printers) {
  web_ui()->CallJavascriptFunction("setPrinters", printers);
}
