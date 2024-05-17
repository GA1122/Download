void DownloadItemImpl::Pause() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  if (IsPaused())
    return;

  switch (state_) {
    case CANCELLED_INTERNAL:
    case COMPLETE_INTERNAL:
    case COMPLETING_INTERNAL:
    case INITIAL_INTERNAL:
    case INTERRUPTED_INTERNAL:
    case INTERRUPTED_TARGET_PENDING_INTERNAL:
    case RESUMING_INTERNAL:
      return;

    case IN_PROGRESS_INTERNAL:
    case TARGET_PENDING_INTERNAL:
      job_->Pause();
      UpdateObservers();
      return;

    case MAX_DOWNLOAD_INTERNAL_STATE:
    case TARGET_RESOLVED_INTERNAL:
      NOTREACHED();
  }
}
