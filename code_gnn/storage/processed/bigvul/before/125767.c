TabSpecificContentSettings::TabSpecificContentSettings(WebContents* tab)
    : content::WebContentsObserver(tab),
      profile_(Profile::FromBrowserContext(tab->GetBrowserContext())),
      allowed_local_shared_objects_(profile_),
      blocked_local_shared_objects_(profile_),
      geolocation_settings_state_(profile_),
      pending_protocol_handler_(ProtocolHandler::EmptyProtocolHandler()),
      previous_protocol_handler_(ProtocolHandler::EmptyProtocolHandler()),
      pending_protocol_handler_setting_(CONTENT_SETTING_DEFAULT),
      load_plugins_link_enabled_(true) {
  ClearBlockedContentSettingsExceptForCookies();
  ClearCookieSpecificContentSettings();

  registrar_.Add(this, chrome::NOTIFICATION_CONTENT_SETTINGS_CHANGED,
                 content::Source<HostContentSettingsMap>(
                     profile_->GetHostContentSettingsMap()));
}
