void SyncBackendHost::Core::DoEncryptDataTypes(
    const syncable::ModelTypeSet& encrypted_types) {
  DCHECK(MessageLoop::current() == host_->core_thread_.message_loop());
  syncapi_->EncryptDataTypes(encrypted_types);
}
