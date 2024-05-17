bool WallpaperManager::ShouldSetDeviceWallpaper(const AccountId& account_id,
                                                std::string* url,
                                                std::string* hash) {
  if (!g_browser_process->platform_part()
           ->browser_policy_connector_chromeos()
           ->IsEnterpriseManaged()) {
    return false;
  }

  const base::DictionaryValue* dict = nullptr;
  if (!CrosSettings::Get()->GetDictionary(kDeviceWallpaperImage, &dict) ||
      !dict->GetStringWithoutPathExpansion("url", url) ||
      !dict->GetStringWithoutPathExpansion("hash", hash)) {
    return false;
  }

  if (user_manager::UserManager::Get()->IsUserLoggedIn())
    return false;

  return true;
}
