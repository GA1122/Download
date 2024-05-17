void DownloadRequestLimiter::TabDownloadState::CancelOnce() {
  bool throttled = NotifyCallbacks(false);
  SetDownloadStatusAndNotify(throttled ? PROMPT_BEFORE_DOWNLOAD
                                       : DOWNLOADS_NOT_ALLOWED);
}
