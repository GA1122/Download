bool MockContentSettingsClient::allowScriptFromSource(
    bool enabled_per_settings,
    const blink::WebURL& script_url) {
  bool allowed = enabled_per_settings && flags_->scripts_allowed();
  if (flags_->dump_web_content_settings_client_callbacks() && delegate_) {
    delegate_->PrintMessage(
        std::string("MockContentSettingsClient: allowScriptFromSource(") +
        NormalizeLayoutTestURL(script_url.string().utf8()) + "): " +
        (allowed ? "true" : "false") + "\n");
  }
  return allowed;
}
