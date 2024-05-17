GDataCache::GDataCache(
    const FilePath& cache_root_path,
    base::SequencedWorkerPool* pool,
    const base::SequencedWorkerPool::SequenceToken& sequence_token)
    : cache_root_path_(cache_root_path),
      cache_paths_(GetCachePaths(cache_root_path_)),
      pool_(pool),
      sequence_token_(sequence_token),
      ui_weak_ptr_factory_(this),
      ui_weak_ptr_(ui_weak_ptr_factory_.GetWeakPtr()) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
}
