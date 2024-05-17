bool ExtensionApiTest::RunExtensionSubtest(const std::string& extension_name,
                                           const std::string& page_url) {
  return RunExtensionSubtest(extension_name, page_url, kFlagEnableFileAccess);
}
