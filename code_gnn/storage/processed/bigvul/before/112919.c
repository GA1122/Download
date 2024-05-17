void GDataCacheMetadataMap::Iterate(const IterateCallback& callback) {
  AssertOnSequencedWorkerPool();

  for (CacheMap::const_iterator iter = cache_map_.begin();
       iter != cache_map_.end(); ++iter) {
    callback.Run(iter->first, iter->second);
   }
 }
