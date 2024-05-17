std::string SyncBackendHost::RestoreEncryptionBootstrapToken() {
  PrefService* prefs = profile_->GetPrefs();
  std::string token = prefs->GetString(prefs::kEncryptionBootstrapToken);
  return token;
}
