void GDataRootDirectory::UpdateCacheMap(const std::string& resource_id,
                                        const std::string& md5,
                                        CacheSubDirectoryType subdir,
                                        int cache_state) {

  CacheMap::iterator iter = cache_map_.find(resource_id);
  if (iter == cache_map_.end()) {   
    DCHECK(cache_state != GDataFile::CACHE_STATE_NONE);
    if (cache_state != GDataFile::CACHE_STATE_NONE) {
      CacheEntry* entry = new CacheEntry(md5, subdir, cache_state);
      cache_map_.insert(std::make_pair(resource_id, entry));
      DVLOG(1) << "Added res_id=" << resource_id
               << ", " << entry->ToString();
    }
  } else {   
    CacheEntry* entry = iter->second;
    if (cache_state == GDataFile::CACHE_STATE_NONE) {
      DVLOG(1) << "Deleting res_id=" << resource_id
               << ", " << entry->ToString();
      delete entry;
      cache_map_.erase(iter);
    } else {   
      entry->md5 = md5;
      entry->sub_dir_type = subdir;
      entry->cache_state = cache_state;
      DVLOG(1) << "Updated res_id=" << resource_id
               << ", " << entry->ToString();
    }
  }
}
