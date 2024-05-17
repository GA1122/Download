DownloadItem::DownloadState DownloadItemImpl::InternalToExternalState(
    DownloadInternalState internal_state) {
  switch (internal_state) {
    case INITIAL_INTERNAL:
    case TARGET_PENDING_INTERNAL:
    case TARGET_RESOLVED_INTERNAL:
    case INTERRUPTED_TARGET_PENDING_INTERNAL:
    case IN_PROGRESS_INTERNAL:
      return IN_PROGRESS;
    case COMPLETING_INTERNAL:
      return IN_PROGRESS;
    case COMPLETE_INTERNAL:
      return COMPLETE;
    case CANCELLED_INTERNAL:
      return CANCELLED;
    case INTERRUPTED_INTERNAL:
      return INTERRUPTED;
    case RESUMING_INTERNAL:
      return IN_PROGRESS;
    case MAX_DOWNLOAD_INTERNAL_STATE:
      break;
  }
  NOTREACHED();
  return MAX_DOWNLOAD_STATE;
}
