bool ExtensionApiTest::RunExtensionSubtest(const std::string& extension_name,
                                           const std::string& page_url,
                                           int flags) {
  DCHECK(!page_url.empty()) << "Argument page_url is required.";
  if (ExtensionSubtestsAreSkipped())
    return true;
  return RunExtensionTestImpl(extension_name, page_url, NULL, flags);
}
