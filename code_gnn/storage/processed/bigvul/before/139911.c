bool MockContentSettingsClient::allowScript(bool enabled_per_settings) {
  return enabled_per_settings && flags_->scripts_allowed();
}
