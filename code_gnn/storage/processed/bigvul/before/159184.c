void DownloadItemImpl::Remove() {
  DVLOG(20) << __func__ << "() download = " << DebugString(true);
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  delegate_->AssertStateConsistent(this);
  InterruptAndDiscardPartialState(DOWNLOAD_INTERRUPT_REASON_USER_CANCELED);
  UpdateObservers();
  delegate_->AssertStateConsistent(this);

  NotifyRemoved();
  delegate_->DownloadRemoved(this);
}
