void SyncBackendHost::SetPassphrase(const std::string& passphrase,
                                    bool is_explicit) {
  if (!IsNigoriEnabled()) {
    LOG(WARNING) << "Silently dropping SetPassphrase request.";
    return;
  }

  DCHECK_EQ(MessageLoop::current(), frontend_loop_);
  if (core_->processing_passphrase()) {
    VLOG(1) << "Attempted to call SetPassphrase while already waiting for "
            << " result from previous SetPassphrase call. Silently dropping.";
    return;
  }
  core_->set_processing_passphrase();

  core_thread_.message_loop()->PostTask(FROM_HERE,
      NewRunnableMethod(core_.get(), &SyncBackendHost::Core::DoSetPassphrase,
                        passphrase, is_explicit));
}
