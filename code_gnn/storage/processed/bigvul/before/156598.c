std::string SessionStore::WriteBatch::PutWithoutUpdatingTracker(
    const sync_pb::SessionSpecifics& specifics) {
  DCHECK(AreValidSpecifics(specifics));

  const std::string storage_key = GetStorageKey(specifics);
  batch_->WriteData(storage_key, specifics.SerializeAsString());
  return storage_key;
}
