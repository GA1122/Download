void CheckClientDownloadRequest::NotifyRequestFinished(
    DownloadCheckResult result,
    DownloadCheckResultReason reason) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  weakptr_factory_.InvalidateWeakPtrs();

  DVLOG(2) << "SafeBrowsing download verdict for: " << item_->DebugString(true)
           << " verdict:" << reason << " result:" << static_cast<int>(result);

  item_->RemoveObserver(this);
}
