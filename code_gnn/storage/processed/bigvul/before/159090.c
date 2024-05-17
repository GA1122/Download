bool DownloadItemImpl::CanResume() const {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  switch (state_) {
    case INITIAL_INTERNAL:
    case COMPLETING_INTERNAL:
    case COMPLETE_INTERNAL:
    case CANCELLED_INTERNAL:
    case RESUMING_INTERNAL:
    case INTERRUPTED_TARGET_PENDING_INTERNAL:
      return false;

    case TARGET_PENDING_INTERNAL:
    case TARGET_RESOLVED_INTERNAL:
    case IN_PROGRESS_INTERNAL:
      return IsPaused();

    case INTERRUPTED_INTERNAL: {
      ResumeMode resume_mode = GetResumeMode();
      return resume_mode == RESUME_MODE_USER_RESTART ||
             resume_mode == RESUME_MODE_USER_CONTINUE;
    }

    case MAX_DOWNLOAD_INTERNAL_STATE:
      NOTREACHED();
  }
  return false;
}
