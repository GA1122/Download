const Extension* ExtensionBrowserTest::LoadExtensionAsComponentWithManifest(
    const base::FilePath& path,
    const base::FilePath::CharType* manifest_relative_path) {
  ExtensionService* service = extensions::ExtensionSystem::Get(
      profile())->extension_service();
  ExtensionRegistry* registry = ExtensionRegistry::Get(profile());

  base::ThreadRestrictions::ScopedAllowIO allow_io;
  std::string manifest;
  if (!base::ReadFileToString(path.Append(manifest_relative_path), &manifest)) {
    return NULL;
  }

  service->component_loader()->set_ignore_whitelist_for_testing(true);
  std::string extension_id = service->component_loader()->Add(manifest, path);
  const Extension* extension =
      registry->enabled_extensions().GetByID(extension_id);
  if (!extension)
    return NULL;
  observer_->set_last_loaded_extension_id(extension->id());
  return extension;
}
