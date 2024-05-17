void WebsiteSettings::OnSitePermissionChanged(ContentSettingsType type,
                                              ContentSetting setting) {
  ContentSettingsTypeHistogram histogram_value =
      ContentSettingTypeToHistogramValue(type);
  DCHECK_NE(histogram_value, CONTENT_SETTINGS_TYPE_HISTOGRAM_INVALID)
      << "Invalid content setting type specified.";
  UMA_HISTOGRAM_ENUMERATION("WebsiteSettings.OriginInfo.PermissionChanged",
                            histogram_value,
                            CONTENT_SETTINGS_HISTOGRAM_NUM_TYPES);

  if (setting == ContentSetting::CONTENT_SETTING_ALLOW) {
    UMA_HISTOGRAM_ENUMERATION(
        "WebsiteSettings.OriginInfo.PermissionChanged.Allowed", histogram_value,
        CONTENT_SETTINGS_HISTOGRAM_NUM_TYPES);
  } else if (setting == ContentSetting::CONTENT_SETTING_BLOCK) {
    UMA_HISTOGRAM_ENUMERATION(
        "WebsiteSettings.OriginInfo.PermissionChanged.Blocked", histogram_value,
        CONTENT_SETTINGS_HISTOGRAM_NUM_TYPES);
    const std::string& rappor_metric = GetRapporMetric(type);
    if (!rappor_metric.empty()) {
      rappor::SampleDomainAndRegistryFromGURL(
          g_browser_process->rappor_service(), rappor_metric, this->site_url_);
    }
  }

  RecordWebsiteSettingsAction(WEBSITE_SETTINGS_CHANGED_PERMISSION);

  ContentSettingsPattern primary_pattern;
  ContentSettingsPattern secondary_pattern;
  switch (type) {
    case CONTENT_SETTINGS_TYPE_GEOLOCATION:
    case CONTENT_SETTINGS_TYPE_MIDI_SYSEX:
    case CONTENT_SETTINGS_TYPE_FULLSCREEN:
      primary_pattern = ContentSettingsPattern::FromURLNoWildcard(site_url_);
      secondary_pattern = ContentSettingsPattern::FromURLNoWildcard(site_url_);
      break;
    case CONTENT_SETTINGS_TYPE_NOTIFICATIONS:
      primary_pattern = ContentSettingsPattern::FromURLNoWildcard(site_url_);
      secondary_pattern = ContentSettingsPattern::Wildcard();
      break;
    case CONTENT_SETTINGS_TYPE_IMAGES:
    case CONTENT_SETTINGS_TYPE_JAVASCRIPT:
    case CONTENT_SETTINGS_TYPE_PLUGINS:
    case CONTENT_SETTINGS_TYPE_POPUPS:
    case CONTENT_SETTINGS_TYPE_MOUSELOCK:
    case CONTENT_SETTINGS_TYPE_AUTOMATIC_DOWNLOADS:
    case CONTENT_SETTINGS_TYPE_PUSH_MESSAGING:
      primary_pattern = ContentSettingsPattern::FromURL(site_url_);
      secondary_pattern = ContentSettingsPattern::Wildcard();
      break;
    case CONTENT_SETTINGS_TYPE_MEDIASTREAM_MIC:
    case CONTENT_SETTINGS_TYPE_MEDIASTREAM_CAMERA:
      primary_pattern = ContentSettingsPattern::FromURLNoWildcard(site_url_);
      secondary_pattern = ContentSettingsPattern::Wildcard();
      break;
    default:
      NOTREACHED() << "ContentSettingsType " << type << "is not supported.";
      break;
  }

  content_settings::SettingInfo info;
  scoped_ptr<base::Value> v =
      content_settings_->GetWebsiteSetting(
          site_url_, site_url_, type, std::string(), &info);
  content_settings_->SetNarrowestWebsiteSetting(
      primary_pattern, secondary_pattern, type, std::string(), setting, info);

  show_info_bar_ = true;

#if defined(OS_MACOSX)
  PresentSitePermissions();
#endif
}
