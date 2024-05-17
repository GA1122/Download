syncable::ModelTypeSet SyncBackendHost::GetEncryptedDataTypes() const {
  DCHECK_GT(initialization_state_, NOT_INITIALIZED);
  return core_->sync_manager()->GetEncryptedDataTypes();
}
