void SetSitesMuted(const TabStripModel& tab_strip,
                   const std::vector<int>& indices,
                   const bool mute) {
  for (int tab_index : indices) {
    content::WebContents* web_contents = tab_strip.GetWebContentsAt(tab_index);
    GURL url = web_contents->GetLastCommittedURL();
    if (url.SchemeIs(content::kChromeUIScheme)) {
      SetTabAudioMuted(web_contents, mute,
                       TabMutedReason::CONTENT_SETTING_CHROME, std::string());
    } else {
      Profile* profile =
          Profile::FromBrowserContext(web_contents->GetBrowserContext());
      HostContentSettingsMap* settings =
          HostContentSettingsMapFactory::GetForProfile(profile);
      ContentSetting setting =
          mute ? CONTENT_SETTING_BLOCK : CONTENT_SETTING_ALLOW;
      if (setting == settings->GetDefaultContentSetting(
                         CONTENT_SETTINGS_TYPE_SOUND, nullptr)) {
        setting = CONTENT_SETTING_DEFAULT;
      }
      settings->SetContentSettingDefaultScope(
          url, url, CONTENT_SETTINGS_TYPE_SOUND, std::string(), setting);
    }
  }
}
