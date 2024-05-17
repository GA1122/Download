bool ExtensionApiTest::RunPlatformAppTestWithFlags(
    const std::string& extension_name, int flags) {
  return RunExtensionTestImpl(
      extension_name, std::string(), NULL, flags | kFlagLaunchPlatformApp);
}
