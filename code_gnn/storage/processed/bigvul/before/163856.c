const Extension* ExtensionBrowserTest::UpdateExtensionWaitForIdle(
    const std::string& id,
    const base::FilePath& path,
    int expected_change) {
  return InstallOrUpdateExtension(id, path, INSTALL_UI_TYPE_NONE,
                                  expected_change, Manifest::INTERNAL,
                                  browser(), Extension::NO_FLAGS, false, false);
}
