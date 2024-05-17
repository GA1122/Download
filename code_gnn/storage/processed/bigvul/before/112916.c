 GDataCacheMetadata::GDataCacheMetadata(
    base::SequencedWorkerPool* pool,
    const base::SequencedWorkerPool::SequenceToken& sequence_token)
    : pool_(pool),
      sequence_token_(sequence_token) {
  AssertOnSequencedWorkerPool();
}
