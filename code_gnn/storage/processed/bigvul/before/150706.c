base::string16 PageInfoUI::PermissionDecisionReasonToUIString(
    Profile* profile,
    const PageInfoUI::PermissionInfo& permission,
    const GURL& url) {
  ContentSetting effective_setting = GetEffectiveSetting(
      profile, permission.type, permission.setting, permission.default_setting);
  int message_id = kInvalidResourceID;
  switch (permission.source) {
    case content_settings::SettingSource::SETTING_SOURCE_POLICY:
      message_id = kPermissionButtonTextIDPolicyManaged[effective_setting];
      break;
    case content_settings::SettingSource::SETTING_SOURCE_EXTENSION:
      message_id = kPermissionButtonTextIDExtensionManaged[effective_setting];
      break;
    default:
      break;
  }

  if (permission.setting == CONTENT_SETTING_BLOCK &&
      PermissionUtil::IsPermission(permission.type)) {
    PermissionResult permission_result =
        PermissionManager::Get(profile)->GetPermissionStatus(permission.type,
                                                             url, url);
    switch (permission_result.source) {
      case PermissionStatusSource::MULTIPLE_DISMISSALS:
        message_id = IDS_PAGE_INFO_PERMISSION_AUTOMATICALLY_BLOCKED;
        break;
      default:
        break;
    }
  }

  if (permission.type == CONTENT_SETTINGS_TYPE_ADS)
    message_id = IDS_PAGE_INFO_PERMISSION_ADS_SUBTITLE;

  if (message_id == kInvalidResourceID)
    return base::string16();
  return l10n_util::GetStringUTF16(message_id);
}
