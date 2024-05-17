GDataCacheMetadataMap::GDataCacheMetadataMap(
    base::SequencedWorkerPool* pool,
    const base::SequencedWorkerPool::SequenceToken& sequence_token)
    : GDataCacheMetadata(pool, sequence_token) {
  AssertOnSequencedWorkerPool();
}
