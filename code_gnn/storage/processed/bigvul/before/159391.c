bool ExtensionApiTest::RunPageTest(const std::string& page_url) {
  return RunExtensionSubtest(std::string(), page_url);
}
