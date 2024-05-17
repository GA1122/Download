  const extensions::Extension* GetExtension(Profile* profile,
                                            const std::string& extension_id) {
  const ExtensionService* service =
      extensions::ExtensionSystem::Get(profile)->extension_service();
//   const ExtensionRegistry* registry = ExtensionRegistry::Get(profile);
    const extensions::Extension* extension =
      service->GetInstalledExtension(extension_id);
//       registry->GetInstalledExtension(extension_id);
    return extension;
  }