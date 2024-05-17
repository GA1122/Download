bool Browser::RequestPpapiBrokerPermission(
    WebContents* web_contents,
    const GURL& url,
    const base::FilePath& plugin_path,
    const base::Callback<void(bool)>& callback) {
  Profile* profile =
      Profile::FromBrowserContext(web_contents->GetBrowserContext());
  if (profile->IsGuestSession()) {
    callback.Run(false);
    return true;
  }

  TabSpecificContentSettings* tab_content_settings =
      TabSpecificContentSettings::FromWebContents(web_contents);

  HostContentSettingsMap* content_settings =
      HostContentSettingsMapFactory::GetForProfile(profile);
  ContentSetting setting = content_settings->GetContentSetting(
      url, url, CONTENT_SETTINGS_TYPE_PPAPI_BROKER, std::string());

  if (setting == CONTENT_SETTING_ASK) {
    base::RecordAction(base::UserMetricsAction("PPAPI.BrokerInfobarDisplayed"));

    content::PluginService* plugin_service =
        content::PluginService::GetInstance();
    content::WebPluginInfo plugin;
    bool success = plugin_service->GetPluginInfoByPath(plugin_path, &plugin);
    DCHECK(success);
    std::unique_ptr<PluginMetadata> plugin_metadata(
        PluginFinder::GetInstance()->GetPluginMetadata(plugin));

    PepperBrokerInfoBarDelegate::Create(
        InfoBarService::FromWebContents(web_contents), url,
        plugin_metadata->name(), content_settings, tab_content_settings,
        callback);
    return true;
  }

  bool allowed = (setting == CONTENT_SETTING_ALLOW);
  base::RecordAction(allowed
                         ? base::UserMetricsAction("PPAPI.BrokerSettingAllow")
                         : base::UserMetricsAction("PPAPI.BrokerSettingDeny"));
  tab_content_settings->SetPepperBrokerAllowed(allowed);
  callback.Run(allowed);
  return true;
}
