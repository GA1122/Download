std::unique_ptr<BlobDataHandle> BlobStorageContext::AddFinishedBlob(
    const BlobDataBuilder& external_builder) {
  TRACE_EVENT0("Blob", "Context::AddFinishedBlob");
  return BuildBlob(external_builder, TransportAllowedCallback());
}
