void WebsiteSettings::RecordWebsiteSettingsAction(
    WebsiteSettingsAction action) {
  UMA_HISTOGRAM_ENUMERATION("WebsiteSettings.Action",
                            action,
                            WEBSITE_SETTINGS_COUNT);


  if (site_url_.SchemeIs(url::kHttpsScheme)) {
    UMA_HISTOGRAM_ENUMERATION("WebsiteSettings.Action.HttpsUrl",
                              action,
                              WEBSITE_SETTINGS_COUNT);
  }
}
