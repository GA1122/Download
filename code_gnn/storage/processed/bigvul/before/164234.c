void AppCacheUpdateJob::BuildUrlFileList(const AppCacheManifest& manifest) {
  for (const std::string& explicit_url : manifest.explicit_urls)
    AddUrlToFileList(GURL(explicit_url), AppCacheEntry::EXPLICIT);

  for (const auto& intercept : manifest.intercept_namespaces)
    AddUrlToFileList(intercept.target_url, AppCacheEntry::INTERCEPT);

  for (const auto& fallback : manifest.fallback_namespaces)
    AddUrlToFileList(fallback.target_url, AppCacheEntry::FALLBACK);

  if (update_type_ == UPGRADE_ATTEMPT) {
    for (const auto& pair : group_->newest_complete_cache()->entries()) {
      const AppCacheEntry& entry = pair.second;
      if (entry.IsMaster())
        AddUrlToFileList(pair.first, AppCacheEntry::MASTER);
    }
  }
}
