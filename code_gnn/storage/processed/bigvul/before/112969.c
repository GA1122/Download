void ChromeDownloadManagerDelegate::ShouldCompleteDownloadInternal(
    int download_id,
    const base::Closure& user_complete_callback) {
  DownloadItem* item = download_manager_->GetDownload(download_id);
  if (!item)
    return;
  if (IsDownloadReadyForCompletion(item, base::Bind(
        &ChromeDownloadManagerDelegate::ShouldCompleteDownloadInternal, this,
        download_id, user_complete_callback)))
    user_complete_callback.Run();
}
