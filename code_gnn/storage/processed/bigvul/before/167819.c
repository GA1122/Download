ContentSetting GetSettingFromDownloadStatus(
    DownloadRequestLimiter::DownloadStatus status) {
  switch (status) {
    case DownloadRequestLimiter::ALLOW_ONE_DOWNLOAD:
    case DownloadRequestLimiter::PROMPT_BEFORE_DOWNLOAD:
      return CONTENT_SETTING_ASK;
    case DownloadRequestLimiter::ALLOW_ALL_DOWNLOADS:
      return CONTENT_SETTING_ALLOW;
    case DownloadRequestLimiter::DOWNLOADS_NOT_ALLOWED:
      return CONTENT_SETTING_BLOCK;
  }
  NOTREACHED();
  return CONTENT_SETTING_DEFAULT;
}
