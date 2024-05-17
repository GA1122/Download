bool SyncTest::IsEncrypted(int index, syncable::ModelType type) {
  return GetClient(index)->IsTypeEncrypted(type);
}
