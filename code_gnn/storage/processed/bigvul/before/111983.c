bool SyncTest::EnableEncryption(int index, syncable::ModelType type) {
  return GetClient(index)->EnableEncryptionForType(type);
}
