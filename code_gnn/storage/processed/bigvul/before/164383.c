 bool ExtensionApiTest::RunExtensionTestIncognito(
     const std::string& extension_name) {
   return RunExtensionTestImpl(extension_name,
                              std::string(),
                              NULL,
                              kFlagEnableIncognito | kFlagEnableFileAccess);
}
