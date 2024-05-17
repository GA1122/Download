std::unique_ptr<BlobDataHandle> BlobStorageContext::BuildBlob(
    const BlobDataBuilder& content,
    const TransportAllowedCallback& transport_allowed_callback) {
  DCHECK(!registry_.HasEntry(content.uuid_));

  BlobEntry* entry = registry_.CreateEntry(
      content.uuid(), content.content_type_, content.content_disposition_);

  return BuildBlobInternal(entry, content, transport_allowed_callback);
}
