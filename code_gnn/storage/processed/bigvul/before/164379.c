bool ExtensionApiTest::RunExtensionSubtest(const std::string& extension_name,
                                           const std::string& page_url,
                                           int flags) {
  return RunExtensionSubtestWithArgAndFlags(extension_name, page_url, nullptr,
                                            flags);
}
