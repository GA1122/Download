void GDataCacheMetadataMap::UpdateCache(const std::string& resource_id,
                                    const std::string& md5,
                                    GDataCache::CacheSubDirectoryType subdir,
                                    int cache_state) {
  AssertOnSequencedWorkerPool();

  CacheMap::iterator iter = cache_map_.find(resource_id);
  if (iter == cache_map_.end()) {   
    DCHECK(cache_state != GDataCache::CACHE_STATE_NONE);
    if (cache_state != GDataCache::CACHE_STATE_NONE) {
      GDataCache::CacheEntry cache_entry(md5, subdir, cache_state);
      cache_map_.insert(std::make_pair(resource_id, cache_entry));
      DVLOG(1) << "Added res_id=" << resource_id
               << ", " << cache_entry.ToString();
    }
  } else {   
    if (cache_state == GDataCache::CACHE_STATE_NONE) {
      DVLOG(1) << "Deleting res_id=" << resource_id
               << ", " << iter->second.ToString();
      cache_map_.erase(iter);
    } else {   
      iter->second.md5 = md5;
      iter->second.sub_dir_type = subdir;
      iter->second.cache_state = cache_state;
      DVLOG(1) << "Updated res_id=" << resource_id
               << ", " << iter->second.ToString();
    }
  }
}
