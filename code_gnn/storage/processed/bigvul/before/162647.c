HeadlessPrintManager::PDFContentsToDictionaryValue(const std::string& data) {
  std::string base_64_data;
  base::Base64Encode(data, &base_64_data);
  auto result = std::make_unique<base::DictionaryValue>();
  result->SetString("data", base_64_data);
  return result;
}
