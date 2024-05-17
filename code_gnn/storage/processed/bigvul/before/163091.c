std::unique_ptr<BlobDataHandle> BlobStorageContext::BuildPreregisteredBlob(
    const BlobDataBuilder& content,
    const TransportAllowedCallback& transport_allowed_callback) {
  BlobEntry* entry = registry_.GetEntry(content.uuid());
  DCHECK(entry);
  DCHECK_EQ(BlobStatus::PENDING_CONSTRUCTION, entry->status());
  entry->set_size(0);

  return BuildBlobInternal(entry, content, transport_allowed_callback);
}
