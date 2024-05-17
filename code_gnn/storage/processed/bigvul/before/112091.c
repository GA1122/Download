void SyncManager::SyncInternal::OnEncryptedTypesChanged(
    syncable::ModelTypeSet encrypted_types,
    bool encrypt_everything) {
  FOR_EACH_OBSERVER(
      SyncManager::Observer, observers_,
      OnEncryptedTypesChanged(encrypted_types, encrypt_everything));
}
