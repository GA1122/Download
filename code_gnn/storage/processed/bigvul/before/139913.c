bool MockContentSettingsClient::allowStorage(bool enabled_per_settings) {
  return flags_->storage_allowed();
}
