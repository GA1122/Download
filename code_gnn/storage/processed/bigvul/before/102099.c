void SyncBackendHost::PersistEncryptionBootstrapToken(
    const std::string& token) {
  PrefService* prefs = profile_->GetPrefs();

  prefs->SetString(prefs::kEncryptionBootstrapToken, token);
  prefs->ScheduleSavePersistentPrefs();
}
