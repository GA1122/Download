void SyncBackendHost::Core::DoSetDecryptionPassphrase(
    const std::string& passphrase) {
  DCHECK_EQ(MessageLoop::current(), sync_loop_);
  sync_manager_->SetDecryptionPassphrase(passphrase);
}
