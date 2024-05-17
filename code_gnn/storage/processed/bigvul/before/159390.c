bool ExtensionApiTest::RunExtensionTestWithFlags(
    const std::string& extension_name,
    int flags) {
  return RunExtensionTestImpl(extension_name, std::string(), nullptr, flags);
}
