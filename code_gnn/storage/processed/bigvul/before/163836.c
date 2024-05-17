const Extension* ExtensionBrowserTest::InstallExtensionFromWebstore(
    const base::FilePath& path,
    int expected_change) {
  return InstallOrUpdateExtension(
      std::string(), path, INSTALL_UI_TYPE_AUTO_CONFIRM, expected_change,
      Manifest::INTERNAL, browser(), Extension::FROM_WEBSTORE, true, false);
}
