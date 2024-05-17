bool ExtensionApiTest::RunPlatformAppTestWithFlags(
    const std::string& extension_name,
    const char* custom_arg,
    int flags) {
  return RunExtensionTestImpl(extension_name, std::string(), custom_arg,
                              flags | kFlagLaunchPlatformApp);
}
