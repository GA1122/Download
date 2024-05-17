void BlobStorageContext::DecrementBlobRefCount(const std::string& uuid) {
  BlobEntry* entry = registry_.GetEntry(uuid);
  DCHECK(entry);
  DCHECK_GT(entry->refcount(), 0u);
  entry->DecrementRefCount();
  if (entry->refcount() == 0) {
    DVLOG(1) << "BlobStorageContext::DecrementBlobRefCount(" << uuid
             << "): Deleting blob.";
    ClearAndFreeMemory(entry);
    registry_.DeleteEntry(uuid);
  }
}
