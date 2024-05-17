bool ExtensionApiTest::RunExtensionTestAllowOldManifestVersion(
    const std::string& extension_name) {
  return RunExtensionTestImpl(
      extension_name,
      std::string(),
      NULL,
      kFlagEnableFileAccess | kFlagAllowOldManifestVersions);
}
