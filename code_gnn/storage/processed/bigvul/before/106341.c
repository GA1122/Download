void SyncBackendHost::Core::NotifyPassphraseRequired(
    sync_api::PassphraseRequiredReason reason) {
  if (!host_ || !host_->frontend_)
    return;

  DCHECK_EQ(MessageLoop::current(), host_->frontend_loop_);

  if (reason == sync_api::REASON_SET_PASSPHRASE_FAILED)
    processing_passphrase_ = false;

  if (processing_passphrase_) {
    VLOG(1) << "Core received OnPassphraseRequired while processing a "
            << "passphrase. Silently dropping.";
    return;
  }

  host_->frontend_->OnPassphraseRequired(reason);
}
