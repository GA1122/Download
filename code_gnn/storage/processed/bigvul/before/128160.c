const extensions::Extension* FindCastExtension() {
  Profile* profile = ProfileManager::GetActiveUserProfile();
  const extensions::ExtensionRegistry* extension_registry =
      extensions::ExtensionRegistry::Get(profile);
  const extensions::ExtensionSet& enabled_extensions =
      extension_registry->enabled_extensions();

  for (size_t i = 0; i < arraysize(extensions::kChromecastExtensionIds); ++i) {
    const std::string extension_id(extensions::kChromecastExtensionIds[i]);
    if (enabled_extensions.Contains(extension_id)) {
      return extension_registry->GetExtensionById(
          extension_id, extensions::ExtensionRegistry::ENABLED);
    }
  }
  return nullptr;
}
