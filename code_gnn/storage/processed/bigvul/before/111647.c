GDataRootDirectory::CacheEntry* GDataRootDirectory::GetCacheEntry(
    const std::string& resource_id,
    const std::string& md5) {

  CacheMap::iterator iter = cache_map_.find(resource_id);
  if (iter == cache_map_.end()) {
    DVLOG(1) << "Can't find " << resource_id << " in cache map";
    return NULL;
  }

  CacheEntry* entry = iter->second;

  if (!entry->IsDirty() && !md5.empty() && entry->md5 != md5) {
    DVLOG(1) << "Non-matching md5: want=" << md5
             << ", found=[res_id=" << resource_id
             << ", " << entry->ToString()
             << "]";
    return NULL;
  }

  DVLOG(1) << "Found entry for res_id=" << resource_id
           << ", " << entry->ToString();

  return entry;
}
