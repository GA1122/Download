void SyncBackendHost::NotifyPassphraseAccepted() {
  if (!frontend_)
    return;

  DCHECK_EQ(MessageLoop::current(), frontend_loop_);

  cached_pending_keys_.clear_blob();
  frontend_->OnPassphraseAccepted();
}
