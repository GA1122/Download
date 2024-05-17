bool ExtensionApiTest::RunComponentExtensionTestWithArg(
    const std::string& extension_name,
    const char* custom_arg) {
  return RunExtensionTestImpl(extension_name, std::string(), custom_arg,
                              kFlagEnableFileAccess | kFlagLoadAsComponent);
}
