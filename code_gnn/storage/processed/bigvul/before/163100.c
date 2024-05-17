std::unique_ptr<BlobDataHandle> BlobStorageContext::GetBlobDataFromUUID(
    const std::string& uuid) {
  BlobEntry* entry = registry_.GetEntry(uuid);
  if (!entry)
    return nullptr;
  return CreateHandle(uuid, entry);
}
