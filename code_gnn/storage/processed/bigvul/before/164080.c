const AppCacheEntry* AppCache::GetEntryAndUrlWithResponseId(
    int64_t response_id,
    GURL* optional_url_out) {
  for (const auto& pair : entries_) {
    if (pair.second.response_id() == response_id) {
      if (optional_url_out)
        *optional_url_out = pair.first;
      return &pair.second;
    }
  }
  return nullptr;
}
