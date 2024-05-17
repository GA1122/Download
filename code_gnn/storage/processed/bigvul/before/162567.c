void Resource::CachedMetadataHandlerImpl::SendToPlatform() {
  if (cached_metadata_) {
    const Vector<char>& serialized_data = cached_metadata_->SerializedData();
    Platform::Current()->CacheMetadata(response_url_, response_time_,
                                       serialized_data.data(),
                                       serialized_data.size());
  } else {
    Platform::Current()->CacheMetadata(response_url_, response_time_, nullptr,
                                       0);
  }
}
