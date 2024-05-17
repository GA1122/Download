void DownloadManagerImpl::SetDelegate(DownloadManagerDelegate* delegate) {
  delegate_ = delegate;

  if (initialized_ || in_progress_cache_initialized_)
    return;

  base::RepeatingClosure post_init_callback = base::BindRepeating(
      &DownloadManagerImpl::PostInitialization, weak_factory_.GetWeakPtr(),
      DOWNLOAD_INITIALIZATION_DEPENDENCY_IN_PROGRESS_CACHE);

  if (delegate_) {
    download::InProgressCache* in_progress_cache =
        delegate_->GetInProgressCache();
    if (in_progress_cache) {
      in_progress_cache->Initialize(post_init_callback);
      return;
    }
  }

  BrowserThread::PostTask(BrowserThread::UI, FROM_HERE, post_init_callback);
}
