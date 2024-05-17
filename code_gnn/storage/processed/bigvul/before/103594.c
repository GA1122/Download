bool ChromeContentBrowserClient::AllowAppCache(
    const GURL& manifest_url,
    const content::ResourceContext& context) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  ProfileIOData* io_data =
      reinterpret_cast<ProfileIOData*>(context.GetUserData(NULL));
  ContentSetting setting = io_data->GetHostContentSettingsMap()->
      GetCookieContentSetting(manifest_url, manifest_url, true);
  DCHECK(setting != CONTENT_SETTING_DEFAULT);
  return setting != CONTENT_SETTING_BLOCK;
}
