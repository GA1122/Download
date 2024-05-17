GDataCache* GDataCache::CreateGDataCacheOnUIThread(
    const FilePath& cache_root_path,
    base::SequencedWorkerPool* pool,
    const base::SequencedWorkerPool::SequenceToken& sequence_token) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  return new GDataCache(cache_root_path, pool, sequence_token);
}
