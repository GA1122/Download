void SyncBackendHost::NotifyPassphraseRequired(
    sync_api::PassphraseRequiredReason reason,
    sync_pb::EncryptedData pending_keys) {
  if (!frontend_)
    return;

  DCHECK_EQ(MessageLoop::current(), frontend_loop_);

  cached_pending_keys_ = pending_keys;

  frontend_->OnPassphraseRequired(reason, pending_keys);
}
