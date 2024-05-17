void DownloadRequestLimiter::TabDownloadState::SetContentSetting(
    ContentSetting setting) {
  if (!web_contents_)
    return;
  HostContentSettingsMap* settings =
      DownloadRequestLimiter::GetContentSettings(web_contents_);
  if (!settings)
    return;
  settings->SetContentSettingDefaultScope(
      web_contents_->GetURL(), GURL(),
      CONTENT_SETTINGS_TYPE_AUTOMATIC_DOWNLOADS, std::string(), setting);
}
