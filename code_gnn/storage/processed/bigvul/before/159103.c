void DownloadItemImpl::DestinationError(
    DownloadInterruptReason reason,
    int64_t bytes_so_far,
    std::unique_ptr<crypto::SecureHash> secure_hash) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DCHECK(state_ == TARGET_PENDING_INTERNAL || state_ == IN_PROGRESS_INTERNAL);
  DVLOG(20) << __func__
            << "() reason:" << DownloadInterruptReasonToString(reason)
            << " this:" << DebugString(true);

  InterruptWithPartialState(bytes_so_far, std::move(secure_hash), reason);
  UpdateObservers();
}
