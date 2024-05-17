bool DownloadProtectionService::IsSupportedDownload(
    const content::DownloadItem& item,
    const base::FilePath& target_path) const {
#if defined(OS_WIN) || defined(OS_MACOSX)
  DownloadCheckResultReason reason = REASON_MAX;
  ClientDownloadRequest::DownloadType type =
      ClientDownloadRequest::WIN_EXECUTABLE;
  return (CheckClientDownloadRequest::IsSupportedDownload(
              item, target_path, &reason, &type) &&
          (ClientDownloadRequest::CHROME_EXTENSION != type));
#else
  return false;
#endif
}
