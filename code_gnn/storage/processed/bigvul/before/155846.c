void SupervisedUserService::OnExtensionInstalled(
    content::BrowserContext* browser_context,
    const extensions::Extension* extension,
    bool is_update) {
  if (!is_update)
    return;

  ExtensionPrefs* extension_prefs = ExtensionPrefs::Get(profile_);
  const std::string& id = extension->id();
  const base::Version& version = extension->version();

  if (!extension_prefs->HasDisableReason(
          id, extensions::disable_reason::DISABLE_PERMISSIONS_INCREASE) &&
      approved_extensions_map_.count(id) > 0 &&
      approved_extensions_map_[id] < version) {
    approved_extensions_map_[id] = version;

    std::string key = SupervisedUserSettingsService::MakeSplitSettingKey(
        supervised_users::kApprovedExtensions, id);
    std::unique_ptr<base::Value> version_value(
        new base::Value(version.GetString()));
    GetSettingsService()->UpdateSetting(key, std::move(version_value));
  }
  ChangeExtensionStateIfNecessary(id);
}
