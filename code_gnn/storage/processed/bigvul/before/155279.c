bool ChromeContentBrowserClient::AllowServiceWorker(
    const GURL& scope,
    const GURL& first_party_url,
    content::ResourceContext* context,
    base::RepeatingCallback<content::WebContents*()> wc_getter) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

#if BUILDFLAG(ENABLE_EXTENSIONS)
  if (!ChromeContentBrowserClientExtensionsPart::AllowServiceWorker(
          scope, first_party_url, context)) {
    return false;
  }
#endif

  ProfileIOData* io_data = ProfileIOData::FromResourceContext(context);

  content_settings::SettingInfo info;
  std::unique_ptr<base::Value> value =
      io_data->GetHostContentSettingsMap()->GetWebsiteSetting(
          first_party_url, first_party_url, CONTENT_SETTINGS_TYPE_JAVASCRIPT,
          std::string(), &info);
  ContentSetting setting = content_settings::ValueToContentSetting(value.get());
  bool allow_javascript = (setting == CONTENT_SETTING_ALLOW);

  bool allow_serviceworker =
      io_data->GetCookieSettings()->IsCookieAccessAllowed(scope,
                                                          first_party_url);
  if (!wc_getter.is_null()) {
    base::PostTaskWithTraits(
        FROM_HERE, {BrowserThread::UI},
        base::BindOnce(&TabSpecificContentSettings::ServiceWorkerAccessed,
                       std::move(wc_getter), scope, !allow_javascript,
                       !allow_serviceworker));
  }
  return allow_javascript && allow_serviceworker;
}
