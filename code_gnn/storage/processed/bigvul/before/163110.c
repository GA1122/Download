bool BlobStorageContext::RegisterPublicBlobURL(const GURL& blob_url,
                                               const std::string& uuid) {
  if (!registry_.CreateUrlMapping(blob_url, uuid))
    return false;
  IncrementBlobRefCount(uuid);
  return true;
}
