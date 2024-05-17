bool ShouldShowPermission(
    const PageInfoUI::PermissionInfo& info,
    const GURL& site_url,
    HostContentSettingsMap* content_settings,
    content::WebContents* web_contents,
    TabSpecificContentSettings* tab_specific_content_settings) {
  if (info.type == CONTENT_SETTINGS_TYPE_ADS) {
    if (!base::FeatureList::IsEnabled(
            subresource_filter::kSafeBrowsingSubresourceFilter)) {
      return false;
    }

    return content_settings->GetWebsiteSetting(
               site_url, GURL(), CONTENT_SETTINGS_TYPE_ADS_DATA, std::string(),
               nullptr) != nullptr;
  }

  if (info.type == CONTENT_SETTINGS_TYPE_SOUND) {
    if (web_contents && web_contents->WasEverAudible())
      return true;
  }

#if defined(OS_ANDROID)
  if (info.type == CONTENT_SETTINGS_TYPE_GEOLOCATION)
    return true;
#else
  if (info.type == CONTENT_SETTINGS_TYPE_PLUGINS &&
      content_settings->GetWebsiteSetting(site_url, site_url,
                                          CONTENT_SETTINGS_TYPE_PLUGINS_DATA,
                                          std::string(), nullptr) != nullptr) {
    return true;
  }
#endif

#if !defined(OS_ANDROID)
  if (info.type == CONTENT_SETTINGS_TYPE_AUTOPLAY)
    return false;
#endif

  if (tab_specific_content_settings->HasContentSettingChangedViaPageInfo(
          info.type)) {
    return true;
  }

  if (!IsPermissionFactoryDefault(content_settings, info))
    return true;

  return false;
}
