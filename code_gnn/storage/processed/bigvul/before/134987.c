void AppCacheUpdateJob::BuildUrlFileList(const AppCacheManifest& manifest) {
  for (base::hash_set<std::string>::const_iterator it =
           manifest.explicit_urls.begin();
       it != manifest.explicit_urls.end(); ++it) {
    AddUrlToFileList(GURL(*it), AppCacheEntry::EXPLICIT);
  }

  const std::vector<AppCacheNamespace>& intercepts =
      manifest.intercept_namespaces;
  for (std::vector<AppCacheNamespace>::const_iterator it = intercepts.begin();
       it != intercepts.end(); ++it) {
    int flags = AppCacheEntry::INTERCEPT;
    if (it->is_executable)
      flags |= AppCacheEntry::EXECUTABLE;
    AddUrlToFileList(it->target_url, flags);
  }

  const std::vector<AppCacheNamespace>& fallbacks =
      manifest.fallback_namespaces;
  for (std::vector<AppCacheNamespace>::const_iterator it = fallbacks.begin();
       it != fallbacks.end(); ++it) {
     AddUrlToFileList(it->target_url, AppCacheEntry::FALLBACK);
  }

  if (update_type_ == UPGRADE_ATTEMPT) {
    const AppCache::EntryMap& entries =
        group_->newest_complete_cache()->entries();
    for (AppCache::EntryMap::const_iterator it = entries.begin();
         it != entries.end(); ++it) {
      const AppCacheEntry& entry = it->second;
      if (entry.IsMaster())
        AddUrlToFileList(it->first, AppCacheEntry::MASTER);
    }
  }
}
