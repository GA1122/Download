void SyncBackendHost::Core::DoSetEncryptionPassphrase(
    const std::string& passphrase,
    bool is_explicit) {
  DCHECK_EQ(MessageLoop::current(), sync_loop_);
  sync_manager_->SetEncryptionPassphrase(passphrase, is_explicit);
}
