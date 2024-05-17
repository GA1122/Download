BlobStatus BlobStorageContext::GetBlobStatus(const std::string& uuid) const {
  const BlobEntry* entry = registry_.GetEntry(uuid);
  if (!entry)
    return BlobStatus::ERR_INVALID_CONSTRUCTION_ARGUMENTS;
  return entry->status();
}
