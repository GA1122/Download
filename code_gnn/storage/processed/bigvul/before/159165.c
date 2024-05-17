bool DownloadItemImpl::IsValidSavePackageStateTransition(
    DownloadInternalState from,
    DownloadInternalState to) {
#if DCHECK_IS_ON()
  switch (from) {
    case INITIAL_INTERNAL:
    case TARGET_PENDING_INTERNAL:
    case INTERRUPTED_TARGET_PENDING_INTERNAL:
    case TARGET_RESOLVED_INTERNAL:
    case COMPLETING_INTERNAL:
    case COMPLETE_INTERNAL:
    case INTERRUPTED_INTERNAL:
    case RESUMING_INTERNAL:
    case CANCELLED_INTERNAL:
      return false;

    case IN_PROGRESS_INTERNAL:
      return to == CANCELLED_INTERNAL || to == COMPLETE_INTERNAL;

    case MAX_DOWNLOAD_INTERNAL_STATE:
      NOTREACHED();
  }
  return false;
#else
  return true;
#endif
}
