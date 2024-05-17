void DownloadItemImpl::OnDownloadRenamedToIntermediateName(
    DownloadInterruptReason reason,
    const base::FilePath& full_path) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(state_ == TARGET_PENDING_INTERNAL ||
         state_ == INTERRUPTED_TARGET_PENDING_INTERNAL);
  DCHECK(download_file_);
  DVLOG(20) << __func__ << "() download=" << DebugString(true);

  if (DOWNLOAD_INTERRUPT_REASON_NONE == reason) {
    SetFullPath(full_path);
  } else {
    deferred_interrupt_reason_ = reason;
    TransitionTo(INTERRUPTED_TARGET_PENDING_INTERNAL);
  }
  OnTargetResolved();
}
