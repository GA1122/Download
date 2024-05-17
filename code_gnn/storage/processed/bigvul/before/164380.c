bool ExtensionApiTest::RunExtensionSubtestWithArg(
    const std::string& extension_name,
    const std::string& page_url,
    const char* custom_arg) {
  return RunExtensionSubtestWithArgAndFlags(extension_name, page_url,
                                            custom_arg, kFlagEnableFileAccess);
}
