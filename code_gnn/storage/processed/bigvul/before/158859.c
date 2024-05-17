bool Browser::ShouldAllowRunningInsecureContent(
    content::WebContents* web_contents,
    bool allowed_per_prefs,
    const url::Origin& origin,
    const GURL& resource_url) {
  FilteredReportInsecureContentRan(resource_url);

  MixedContentSettingsTabHelper* mixed_content_settings =
      MixedContentSettingsTabHelper::FromWebContents(web_contents);
  DCHECK(mixed_content_settings);
  if (allowed_per_prefs ||
      mixed_content_settings->is_running_insecure_content_allowed()) {
    return true;
  }

  if (!origin.host().empty()) {
    TabSpecificContentSettings* tab_settings =
        TabSpecificContentSettings::FromWebContents(web_contents);
    DCHECK(tab_settings);
    tab_settings->OnContentBlockedWithDetail(CONTENT_SETTINGS_TYPE_MIXEDSCRIPT,
                                             base::UTF8ToUTF16(origin.host()));
  }
  return false;
}
