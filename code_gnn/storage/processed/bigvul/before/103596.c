bool ChromeContentBrowserClient::AllowGetCookie(
    const GURL& url,
    const GURL& first_party,
    const net::CookieList& cookie_list,
    const content::ResourceContext& context,
    int render_process_id,
    int render_view_id) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  ProfileIOData* io_data =
      reinterpret_cast<ProfileIOData*>(context.GetUserData(NULL));
  ContentSetting setting = io_data->GetHostContentSettingsMap()->
      GetCookieContentSetting(url, first_party, false);
  bool allow = setting == CONTENT_SETTING_ALLOW ||
      setting == CONTENT_SETTING_SESSION_ONLY;

  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      NewRunnableFunction(
          &TabSpecificContentSettings::CookiesRead,
          render_process_id, render_view_id, url, cookie_list, !allow));
  return allow;
}
