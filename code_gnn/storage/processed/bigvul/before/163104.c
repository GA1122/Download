void BlobStorageContext::NotifyTransportComplete(const std::string& uuid) {
  BlobEntry* entry = registry_.GetEntry(uuid);
  CHECK(entry) << "There is no blob entry with uuid " << uuid;
  DCHECK(BlobStatusIsPending(entry->status()));
  NotifyTransportCompleteInternal(entry);
}
