ExtensionSyncData ExtensionService::GetSyncDataHelper(
    const Extension& extension) const {
  const std::string& id = extension.id();
  ExtensionSyncData data;
  data.id = id;
  data.uninstalled = false;
  data.enabled = IsExtensionEnabled(id);
  data.incognito_enabled = IsIncognitoEnabled(id);
  data.version = *extension.version();
  data.update_url = extension.update_url();
  data.name = extension.name();
  return data;
}
