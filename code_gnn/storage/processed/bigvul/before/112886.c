void GDataCache::Initialize() {
  AssertOnSequencedWorkerPool();

  GDataCacheMetadataMap* cache_data =
      new GDataCacheMetadataMap(pool_, sequence_token_);
  cache_data->Initialize(cache_paths_);
  metadata_.reset(cache_data);
}
