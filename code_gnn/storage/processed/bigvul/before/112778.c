void PrintPreviewHandler::SendFailedToGetPrinterCapabilities(
    const std::string& printer_name) {
  VLOG(1) << "Get printer capabilities failed";
  base::StringValue printer_name_value(printer_name);
  web_ui()->CallJavascriptFunction("failedToGetPrinterCapabilities",
                                   printer_name_value);
}
