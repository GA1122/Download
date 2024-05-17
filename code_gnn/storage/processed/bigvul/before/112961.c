void ChromeDownloadManagerDelegate::OnPathExistenceAvailable(
    int32 download_id,
    const FilePath& target_path,
    DownloadItem::TargetDisposition disposition,
    content::DownloadDangerType danger_type) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));
  DownloadItem* download =
      download_manager_->GetActiveDownloadItem(download_id);
  if (!download)
    return;
  download->OnTargetPathDetermined(target_path, disposition, danger_type);
  download_manager_->RestartDownload(download_id);
}
