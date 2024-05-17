bool ChromeContentBrowserClient::AllowSetCookie(
    const GURL& url,
    const GURL& first_party,
    const std::string& cookie_line,
    const content::ResourceContext& context,
    int render_process_id,
    int render_view_id,
    net::CookieOptions* options) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  ProfileIOData* io_data =
      reinterpret_cast<ProfileIOData*>(context.GetUserData(NULL));
  ContentSetting setting = io_data->GetHostContentSettingsMap()->
      GetCookieContentSetting(url, first_party, true);

  if (setting == CONTENT_SETTING_SESSION_ONLY)
    options->set_force_session();

  bool allow = setting == CONTENT_SETTING_ALLOW ||
      setting == CONTENT_SETTING_SESSION_ONLY;

  BrowserThread::PostTask(
      BrowserThread::UI, FROM_HERE,
      NewRunnableFunction(
          &TabSpecificContentSettings::CookieChanged,
          render_process_id, render_view_id, url, cookie_line, *options,
          !allow));
  return allow;
}
