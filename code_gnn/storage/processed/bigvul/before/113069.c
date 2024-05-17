void DownloadItemImpl::Interrupt(content::DownloadInterruptReason reason) {
  DCHECK(IsInProgress());
  last_reason_ = reason;
  TransitionTo(INTERRUPTED);
  download_stats::RecordDownloadInterrupted(
      reason, received_bytes_, total_bytes_);
  delegate_->DownloadStopped(this);
}
