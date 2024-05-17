DownloadRequestLimiter::DownloadStatus GetDownloadStatusFromSetting(
    ContentSetting setting) {
  switch (setting) {
    case CONTENT_SETTING_ALLOW:
      return DownloadRequestLimiter::ALLOW_ALL_DOWNLOADS;
    case CONTENT_SETTING_BLOCK:
      return DownloadRequestLimiter::DOWNLOADS_NOT_ALLOWED;
    case CONTENT_SETTING_DEFAULT:
    case CONTENT_SETTING_ASK:
      return DownloadRequestLimiter::PROMPT_BEFORE_DOWNLOAD;
    case CONTENT_SETTING_SESSION_ONLY:
    case CONTENT_SETTING_NUM_SETTINGS:
    case CONTENT_SETTING_DETECT_IMPORTANT_CONTENT:
      NOTREACHED();
      return DownloadRequestLimiter::PROMPT_BEFORE_DOWNLOAD;
  }
  NOTREACHED();
  return DownloadRequestLimiter::PROMPT_BEFORE_DOWNLOAD;
}
