HostContentSettingsMap* DownloadRequestLimiter::GetContentSettings(
    content::WebContents* contents) {
  return HostContentSettingsMapFactory::GetForProfile(
      Profile::FromBrowserContext(contents->GetBrowserContext()));
}
