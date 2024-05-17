void PrintPreviewHandler::HandleSaveLastPrinter(const ListValue* args) {
  std::string data_to_save;
  if (args->GetString(0, &data_to_save) && !data_to_save.empty())
    GetStickySettings()->StorePrinterName(data_to_save);

  if (args->GetString(1, &data_to_save) && !data_to_save.empty())
    GetStickySettings()->StoreCloudPrintData(data_to_save);
}
