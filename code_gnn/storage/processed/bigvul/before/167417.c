bool DevToolsDownloadManagerDelegate::ShouldOpenDownload(
    content::DownloadItem* item,
    const content::DownloadOpenDelayedCallback& callback) {
  DevToolsDownloadManagerHelper* download_helper =
      DevToolsDownloadManagerHelper::FromWebContents(item->GetWebContents());

  if (download_helper)
    return true;
  if (proxy_download_delegate_)
    return proxy_download_delegate_->ShouldOpenDownload(item, callback);
  return false;
}
