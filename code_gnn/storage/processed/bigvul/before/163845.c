const Extension* ExtensionBrowserTest::LoadExtensionWithFlags(
    const base::FilePath& path, int flags) {
  return LoadExtensionWithInstallParam(path, flags, std::string());
}
