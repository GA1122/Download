void SyncBackendHost::PersistEncryptionBootstrapToken(
    const std::string& token) {
  CHECK(sync_prefs_.get());
  sync_prefs_->SetEncryptionBootstrapToken(token);
}
