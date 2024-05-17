void DownloadItemImplDelegate::DetermineDownloadTarget(
    DownloadItemImpl* download, const DownloadTargetCallback& callback) {
  base::FilePath target_path(download->GetForcedFilePath());
  callback.Run(target_path, DownloadItem::TARGET_DISPOSITION_OVERWRITE,
               DOWNLOAD_DANGER_TYPE_NOT_DANGEROUS, target_path,
               DOWNLOAD_INTERRUPT_REASON_NONE);
}
