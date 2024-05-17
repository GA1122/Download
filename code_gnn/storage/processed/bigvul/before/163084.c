std::unique_ptr<BlobDataHandle> BlobStorageContext::AddFinishedBlob(
    const BlobDataBuilder* builder) {
  DCHECK(builder);
  return AddFinishedBlob(*builder);
}
