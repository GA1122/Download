bool ExtensionApiTest::RunComponentExtensionTest(
    const std::string& extension_name) {
  return RunExtensionTestImpl(extension_name,
                              std::string(),
                              NULL,
                              kFlagEnableFileAccess | kFlagLoadAsComponent);
}
