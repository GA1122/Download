bool Extension::LoadOfflineEnabled(string16* error) {
  if (!manifest_->HasKey(keys::kOfflineEnabled)) {
    offline_enabled_ = is_platform_app();
    return true;
  }
  if (!manifest_->GetBoolean(keys::kOfflineEnabled, &offline_enabled_)) {
    *error = ASCIIToUTF16(errors::kInvalidOfflineEnabled);
    return false;
  }
  return true;
}
