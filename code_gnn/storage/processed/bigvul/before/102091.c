void SyncBackendHost::Core::OnEncryptionComplete(
    const syncable::ModelTypeSet& encrypted_types) {
  host_->frontend_loop_->PostTask(
      FROM_HERE,
      NewRunnableMethod(this, &Core::NotifyEncryptionComplete,
                        encrypted_types));
}
