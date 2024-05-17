void SyncBackendHost::Core::DoSetPassphrase(const std::string& passphrase,
                                            bool is_explicit) {
  DCHECK(MessageLoop::current() == host_->core_thread_.message_loop());
  syncapi_->SetPassphrase(passphrase, is_explicit);
}
