syncable::ModelTypeSet SyncManager::GetEncryptedDataTypes() const {
  ReadTransaction trans(FROM_HERE, GetUserShare());
  return GetEncryptedTypes(&trans);
}
