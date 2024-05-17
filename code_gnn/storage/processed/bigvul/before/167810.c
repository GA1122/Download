void DownloadRequestLimiter::TabDownloadState::Cancel() {
  SetContentSetting(CONTENT_SETTING_BLOCK);
  bool throttled = NotifyCallbacks(false);
  SetDownloadStatusAndNotify(throttled ? PROMPT_BEFORE_DOWNLOAD
                                       : DOWNLOADS_NOT_ALLOWED);
}
