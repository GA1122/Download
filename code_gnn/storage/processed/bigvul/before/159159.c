bool DownloadItemImpl::IsDone() const {
  switch (state_) {
    case INITIAL_INTERNAL:
    case COMPLETING_INTERNAL:
    case RESUMING_INTERNAL:
    case TARGET_PENDING_INTERNAL:
    case INTERRUPTED_TARGET_PENDING_INTERNAL:
    case TARGET_RESOLVED_INTERNAL:
    case IN_PROGRESS_INTERNAL:
      return false;

    case COMPLETE_INTERNAL:
    case CANCELLED_INTERNAL:
      return true;

    case INTERRUPTED_INTERNAL:
      return !CanResume();

    case MAX_DOWNLOAD_INTERNAL_STATE:
      NOTREACHED();
  }
  return false;
}
