ChromeContentBrowserClient::AllowWebBluetooth(
    content::BrowserContext* browser_context,
    const url::Origin& requesting_origin,
    const url::Origin& embedding_origin) {
  if (variations::GetVariationParamValue(
          PermissionContextBase::kPermissionsKillSwitchFieldStudy,
          "Bluetooth") ==
      PermissionContextBase::kPermissionsKillSwitchBlockedValue) {
    return AllowWebBluetoothResult::BLOCK_GLOBALLY_DISABLED;
  }

  const HostContentSettingsMap* const content_settings =
      HostContentSettingsMapFactory::GetForProfile(
          Profile::FromBrowserContext(browser_context));

  if (content_settings->GetContentSetting(
          requesting_origin.GetURL(), embedding_origin.GetURL(),
          CONTENT_SETTINGS_TYPE_BLUETOOTH_GUARD,
          std::string()) == CONTENT_SETTING_BLOCK) {
    return AllowWebBluetoothResult::BLOCK_POLICY;
  }
  return AllowWebBluetoothResult::ALLOW;
}
