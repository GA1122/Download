void SyncBackendHost::Core::DoSetPassphrase(const std::string& passphrase,
                                            bool is_explicit) {
  DCHECK(MessageLoop::current() == host_->sync_thread_.message_loop());
  sync_manager_->SetPassphrase(passphrase, is_explicit);
}
