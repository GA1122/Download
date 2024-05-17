void BlobStorageContext::RevokePublicBlobURL(const GURL& blob_url) {
  std::string uuid;
  if (!registry_.DeleteURLMapping(blob_url, &uuid))
    return;
  DecrementBlobRefCount(uuid);
}
