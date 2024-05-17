void DownloadRequestLimiter::TabDownloadState::Accept() {
  SetContentSetting(CONTENT_SETTING_ALLOW);
  bool throttled = NotifyCallbacks(true);
  SetDownloadStatusAndNotify(throttled ? PROMPT_BEFORE_DOWNLOAD
                                       : ALLOW_ALL_DOWNLOADS);
}
