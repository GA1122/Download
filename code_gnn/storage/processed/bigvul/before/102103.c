void SyncBackendHost::SetPassphrase(const std::string& passphrase,
                                    bool is_explicit) {
  if (!IsNigoriEnabled()) {
    SLOG(WARNING) << "Silently dropping SetPassphrase request.";
    return;
  }

  DCHECK_EQ(MessageLoop::current(), frontend_loop_);

  sync_thread_.message_loop()->PostTask(FROM_HERE,
      NewRunnableMethod(core_.get(), &SyncBackendHost::Core::DoSetPassphrase,
                        passphrase, is_explicit));
}
