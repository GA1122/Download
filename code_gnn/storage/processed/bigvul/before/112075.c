syncable::ModelTypeSet SyncManager::GetEncryptedDataTypesForTest() const {
  ReadTransaction trans(FROM_HERE, GetUserShare());
  return GetEncryptedTypes(&trans);
}
