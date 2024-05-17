bool DownloadItemImpl::IsValidStateTransition(DownloadInternalState from,
                                              DownloadInternalState to) {
#if DCHECK_IS_ON()
  switch (from) {
    case INITIAL_INTERNAL:
      return to == TARGET_PENDING_INTERNAL ||
             to == INTERRUPTED_TARGET_PENDING_INTERNAL;

    case TARGET_PENDING_INTERNAL:
      return to == INTERRUPTED_TARGET_PENDING_INTERNAL ||
             to == TARGET_RESOLVED_INTERNAL || to == CANCELLED_INTERNAL;

    case INTERRUPTED_TARGET_PENDING_INTERNAL:
      return to == TARGET_RESOLVED_INTERNAL || to == CANCELLED_INTERNAL;

    case TARGET_RESOLVED_INTERNAL:
      return to == IN_PROGRESS_INTERNAL || to == INTERRUPTED_INTERNAL ||
             to == CANCELLED_INTERNAL;

    case IN_PROGRESS_INTERNAL:
      return to == COMPLETING_INTERNAL || to == CANCELLED_INTERNAL ||
             to == INTERRUPTED_INTERNAL;

    case COMPLETING_INTERNAL:
      return to == COMPLETE_INTERNAL;

    case COMPLETE_INTERNAL:
      return false;

    case INTERRUPTED_INTERNAL:
      return to == RESUMING_INTERNAL || to == CANCELLED_INTERNAL;

    case RESUMING_INTERNAL:
      return to == TARGET_PENDING_INTERNAL ||
             to == INTERRUPTED_TARGET_PENDING_INTERNAL ||
             to == TARGET_RESOLVED_INTERNAL || to == CANCELLED_INTERNAL;

    case CANCELLED_INTERNAL:
      return false;

    case MAX_DOWNLOAD_INTERNAL_STATE:
      NOTREACHED();
  }
  return false;
#else
  return true;
#endif   
}
