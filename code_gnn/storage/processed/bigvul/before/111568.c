  bool CacheEntryExists(const std::string& resource_id,
                        const std::string& md5) {
    GDataRootDirectory::CacheEntry* entry =
        file_system_->root_->GetCacheEntry(resource_id, md5);
    return entry != NULL;
  }
