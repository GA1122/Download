void SyncBackendHost::Core::NotifyEncryptionComplete(
    const syncable::ModelTypeSet& encrypted_types) {
  if (!host_)
    return;
  DCHECK_EQ(MessageLoop::current(), host_->frontend_loop_);
  host_->frontend_->OnEncryptionComplete(encrypted_types);
}
