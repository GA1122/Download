void DownloadItemImpl::OnTargetResolved() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DVLOG(20) << __func__ << "() download=" << DebugString(true);
  DCHECK((state_ == TARGET_PENDING_INTERNAL &&
          deferred_interrupt_reason_ == DOWNLOAD_INTERRUPT_REASON_NONE) ||
         (state_ == INTERRUPTED_TARGET_PENDING_INTERNAL &&
          deferred_interrupt_reason_ != DOWNLOAD_INTERRUPT_REASON_NONE))
      << " deferred_interrupt_reason_:"
      << DownloadInterruptReasonToString(deferred_interrupt_reason_)
      << " this:" << DebugString(true);

  TransitionTo(TARGET_RESOLVED_INTERNAL);

  if (DOWNLOAD_INTERRUPT_REASON_NONE != deferred_interrupt_reason_) {
    InterruptWithPartialState(GetReceivedBytes(), std::move(hash_state_),
                              deferred_interrupt_reason_);
    deferred_interrupt_reason_ = DOWNLOAD_INTERRUPT_REASON_NONE;
    UpdateObservers();
    return;
  }

  TransitionTo(IN_PROGRESS_INTERNAL);
  UpdateObservers();
  MaybeCompleteDownload();
}
