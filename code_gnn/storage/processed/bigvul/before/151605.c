void ResourceLoader::DidReceiveCachedMetadata(const char* data, int length) {
  resource_->SetSerializedCachedMetadata(data, length);
}
