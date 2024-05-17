bool AppIsDefault(ExtensionService* service, const std::string& id) {
  return service && extensions::ExtensionPrefs::Get(service->profile())
                        ->WasInstalledByDefault(id);
}
