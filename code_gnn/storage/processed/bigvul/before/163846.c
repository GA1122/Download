const Extension* ExtensionBrowserTest::LoadExtensionWithInstallParam(
    const base::FilePath& path,
    int flags,
    const std::string& install_param) {
  extensions::ChromeTestExtensionLoader loader(profile());
  loader.set_require_modern_manifest_version(
      (flags & kFlagAllowOldManifestVersions) == 0);
  loader.set_ignore_manifest_warnings(
      (flags & kFlagIgnoreManifestWarnings) != 0);
  loader.set_allow_incognito_access((flags & kFlagEnableIncognito) != 0);
  loader.set_allow_file_access((flags & kFlagEnableFileAccess) != 0);
  loader.set_install_param(install_param);
  scoped_refptr<const Extension> extension = loader.LoadExtension(path);
  if (extension)
    observer_->set_last_loaded_extension_id(extension->id());
  return extension.get();
}
