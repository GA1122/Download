std::unique_ptr<BlobDataHandle> BlobStorageContext::AddBrokenBlob(
    const std::string& uuid,
    const std::string& content_type,
    const std::string& content_disposition,
    BlobStatus reason) {
  DCHECK(!registry_.HasEntry(uuid));
  DCHECK(BlobStatusIsError(reason));
  BlobEntry* entry =
      registry_.CreateEntry(uuid, content_type, content_disposition);
  entry->set_status(reason);
  FinishBuilding(entry);
  return CreateHandle(uuid, entry);
}
