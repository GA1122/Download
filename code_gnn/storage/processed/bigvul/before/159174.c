void DownloadItemImpl::OnDownloadFileInitialized(
    DownloadInterruptReason result) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(state_ == TARGET_PENDING_INTERNAL ||
         state_ == INTERRUPTED_TARGET_PENDING_INTERNAL)
      << "Unexpected state: " << DebugDownloadStateString(state_);

  DVLOG(20) << __func__
            << "() result:" << DownloadInterruptReasonToString(result);
  if (result != DOWNLOAD_INTERRUPT_REASON_NONE) {
    ReleaseDownloadFile(true);
    InterruptAndDiscardPartialState(result);
  }

  DetermineDownloadTarget();
}
