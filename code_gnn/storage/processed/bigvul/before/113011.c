void DownloadItemImpl::Cancel(bool user_cancel) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  last_reason_ = user_cancel ?
      content::DOWNLOAD_INTERRUPT_REASON_USER_CANCELED :
      content::DOWNLOAD_INTERRUPT_REASON_USER_SHUTDOWN;

  VLOG(20) << __FUNCTION__ << "() download = " << DebugString(true);
  if (!IsPartialDownload()) {
    return;
  }

  download_stats::RecordDownloadCount(download_stats::CANCELLED_COUNT);

  TransitionTo(CANCELLED);
  if (user_cancel)
    delegate_->DownloadStopped(this);
}
