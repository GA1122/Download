bool ExtensionApiTest::RunPageTest(const std::string& page_url,
                                   int flags) {
  return RunExtensionSubtest(std::string(), page_url, flags);
}
