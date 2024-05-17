bool DownloadManagerImpl::ShouldClearDownloadFromDB(
    const GURL& url,
    download::DownloadItem::DownloadState state,
    download::DownloadInterruptReason reason) {
  if (state == download::DownloadItem::CANCELLED) {
    ++cancelled_download_cleared_from_history_;
    return true;
  }
  if (reason != download::DOWNLOAD_INTERRUPT_REASON_NONE &&
      download::GetDownloadResumeMode(url, reason, false  ,
                                      false  ) ==
          download::ResumeMode::INVALID) {
    ++interrupted_download_cleared_from_history_;
    return true;
  }
  return false;
}
