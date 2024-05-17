void SyncManager::RefreshEncryption() {
  if (data_->UpdateCryptographerFromNigori())
    data_->EncryptDataTypes(syncable::ModelTypeSet());
}
