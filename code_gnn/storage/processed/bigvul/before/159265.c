void DownloadManagerImpl::PostInitialization(
    DownloadInitializationDependency dependency) {
  if (initialized_)
    return;

  switch (dependency) {
    case DOWNLOAD_INITIALIZATION_DEPENDENCY_HISTORY_DB:
      history_db_initialized_ = true;
      break;
    case DOWNLOAD_INITIALIZATION_DEPENDENCY_IN_PROGRESS_CACHE:
      in_progress_cache_initialized_ = true;
      break;
    case DOWNLOAD_INITIALIZATION_DEPENDENCY_NONE:
    default:
      NOTREACHED();
      break;
  }

  initialized_ = history_db_initialized_ && in_progress_cache_initialized_;

  if (initialized_) {
    for (auto& observer : observers_)
      observer.OnManagerInitialized();
  }
}
