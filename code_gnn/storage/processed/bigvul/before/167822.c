void DownloadRequestLimiter::TabDownloadState::OnContentSettingChanged(
    const ContentSettingsPattern& primary_pattern,
    const ContentSettingsPattern& secondary_pattern,
    ContentSettingsType content_type,
    const std::string& resource_identifier) {
  if (content_type != CONTENT_SETTINGS_TYPE_AUTOMATIC_DOWNLOADS)
    return;

  const ContentSettingsDetails details(primary_pattern, secondary_pattern,
                                       content_type, resource_identifier);
  const NavigationController& controller = web_contents()->GetController();

  NavigationEntry* entry = controller.GetVisibleEntry();
  GURL entry_url;
  if (entry)
    entry_url = entry->GetURL();
  if (!details.update_all() && !details.primary_pattern().Matches(entry_url))
    return;

  HostContentSettingsMap* content_settings = GetContentSettings(web_contents());
  if (!content_settings)
    return;

  ContentSetting setting = content_settings->GetContentSetting(
      web_contents()->GetURL(), web_contents()->GetURL(),
      CONTENT_SETTINGS_TYPE_AUTOMATIC_DOWNLOADS, std::string());

  SetDownloadStatusAndNotifyImpl(GetDownloadStatusFromSetting(setting),
                                 setting);
}
