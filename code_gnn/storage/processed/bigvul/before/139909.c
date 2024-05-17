bool MockContentSettingsClient::allowPlugins(bool enabled_per_settings) {
  return enabled_per_settings && flags_->plugins_allowed();
}
