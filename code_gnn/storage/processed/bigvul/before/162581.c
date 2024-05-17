void Resource::SetSerializedCachedMetadata(const char* data, size_t size) {
  DCHECK(!is_revalidating_);
  DCHECK(!GetResponse().IsNull());
  if (cache_handler_)
    cache_handler_->SetSerializedCachedMetadata(data, size);
}
