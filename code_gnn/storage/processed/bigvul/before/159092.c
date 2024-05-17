void DownloadItemImpl::Cancel(bool user_cancel) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DVLOG(20) << __func__ << "() download = " << DebugString(true);
  InterruptAndDiscardPartialState(
      user_cancel ? DOWNLOAD_INTERRUPT_REASON_USER_CANCELED
                  : DOWNLOAD_INTERRUPT_REASON_USER_SHUTDOWN);
  UpdateObservers();
}
