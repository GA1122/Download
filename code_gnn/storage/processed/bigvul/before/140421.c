PermissionUtil::ScopedRevocationReporter::~ScopedRevocationReporter() {
  if (!is_initially_allowed_)
    return;
  HostContentSettingsMap* settings_map =
      HostContentSettingsMapFactory::GetForProfile(profile_);
  ContentSetting final_content_setting = settings_map->GetContentSetting(
      primary_url_, secondary_url_, content_type_, std::string());
  if (final_content_setting != CONTENT_SETTING_ALLOW) {
    PermissionUmaUtil::PermissionRevoked(content_type_, source_ui_,
                                         primary_url_, profile_);
  }
}
