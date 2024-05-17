void PageInfo::OnSitePermissionChanged(ContentSettingsType type,
                                       ContentSetting setting) {
  tab_specific_content_settings()->ContentSettingChangedViaPageInfo(type);

  size_t num_values;
  int histogram_value = ContentSettingTypeToHistogramValue(type, &num_values);
  UMA_HISTOGRAM_EXACT_LINEAR("WebsiteSettings.OriginInfo.PermissionChanged",
                             histogram_value, num_values);

  if (setting == ContentSetting::CONTENT_SETTING_ALLOW) {
    UMA_HISTOGRAM_EXACT_LINEAR(
        "WebsiteSettings.OriginInfo.PermissionChanged.Allowed", histogram_value,
        num_values);

    if (type == CONTENT_SETTINGS_TYPE_PLUGINS) {
      rappor::SampleDomainAndRegistryFromGURL(
          g_browser_process->rappor_service(),
          "ContentSettings.Plugins.AddedAllowException", site_url_);
    }
  } else if (setting == ContentSetting::CONTENT_SETTING_BLOCK) {
    UMA_HISTOGRAM_EXACT_LINEAR(
        "WebsiteSettings.OriginInfo.PermissionChanged.Blocked", histogram_value,
        num_values);
  }

  RecordPageInfoAction(PAGE_INFO_CHANGED_PERMISSION);
  if (type == CONTENT_SETTINGS_TYPE_SOUND) {
    ContentSetting default_setting =
        content_settings_->GetDefaultContentSetting(CONTENT_SETTINGS_TYPE_SOUND,
                                                    nullptr);
    bool mute = (setting == CONTENT_SETTING_BLOCK) ||
                (setting == CONTENT_SETTING_DEFAULT &&
                 default_setting == CONTENT_SETTING_BLOCK);
    if (mute) {
      base::RecordAction(
          base::UserMetricsAction("SoundContentSetting.MuteBy.PageInfo"));
    } else {
      base::RecordAction(
          base::UserMetricsAction("SoundContentSetting.UnmuteBy.PageInfo"));
    }
  }

  PermissionUtil::ScopedRevocationReporter scoped_revocation_reporter(
      profile_, site_url_, site_url_, type, PermissionSourceUI::OIB);

  if (setting != CONTENT_SETTING_BLOCK) {
    PermissionDecisionAutoBlocker::GetForProfile(profile_)->RemoveEmbargoByUrl(
        site_url_, type);
  }
  content_settings_->SetNarrowestContentSetting(site_url_, site_url_, type,
                                                setting);

  if (type != CONTENT_SETTINGS_TYPE_SOUND)
    show_info_bar_ = true;

  PresentSitePermissions();
}
