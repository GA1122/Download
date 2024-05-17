void DownloadRequestLimiter::TabDownloadState::SetDownloadStatusAndNotify(
    DownloadStatus status) {
  SetDownloadStatusAndNotifyImpl(status, GetSettingFromDownloadStatus(status));
}
