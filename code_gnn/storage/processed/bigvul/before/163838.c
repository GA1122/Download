const Extension* ExtensionBrowserTest::InstallOrUpdateExtension(
    const std::string& id,
    const base::FilePath& path,
    InstallUIType ui_type,
    int expected_change,
    Manifest::Location install_source) {
  return InstallOrUpdateExtension(id, path, ui_type, expected_change,
                                  install_source, browser(),
                                  Extension::NO_FLAGS, true, false);
}
