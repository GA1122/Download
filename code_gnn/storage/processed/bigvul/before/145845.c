void PDFCreated(
    const content::DevToolsManagerDelegate::CommandCallback& callback,
    int command_id,
    HeadlessPrintManager::PrintResult print_result,
    const std::string& data) {
  std::unique_ptr<base::DictionaryValue> response;
  if (print_result == HeadlessPrintManager::PRINT_SUCCESS) {
    response = CreateSuccessResponse(
        command_id, HeadlessPrintManager::PDFContentsToDictionaryValue(data));
  } else {
    response = CreateErrorResponse(
        command_id, kErrorServerError,
        HeadlessPrintManager::PrintResultToString(print_result));
  }
  callback.Run(std::move(response));
}
