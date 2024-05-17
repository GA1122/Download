void Resource::ServiceWorkerResponseCachedMetadataHandler::SendToPlatform() {
  if (cache_storage_cache_name_.IsNull())
    return;

  if (cached_metadata_) {
    const Vector<char>& serialized_data = cached_metadata_->SerializedData();
    Platform::Current()->CacheMetadataInCacheStorage(
        response_url_, response_time_, serialized_data.data(),
        serialized_data.size(), WebSecurityOrigin(security_origin_),
        cache_storage_cache_name_);
  } else {
    Platform::Current()->CacheMetadataInCacheStorage(
        response_url_, response_time_, nullptr, 0,
        WebSecurityOrigin(security_origin_), cache_storage_cache_name_);
  }
}
