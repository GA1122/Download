bool MockContentSettingsClient::allowRunningInsecureContent(
    bool enabled_per_settings,
    const blink::WebSecurityOrigin& context,
    const blink::WebURL& url) {
  return enabled_per_settings || flags_->running_insecure_content_allowed();
 }
