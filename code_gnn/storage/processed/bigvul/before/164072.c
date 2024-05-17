void DownloadManagerImpl::StartDownloadItem(
    std::unique_ptr<download::DownloadCreateInfo> info,
    const download::DownloadUrlParameters::OnStartedCallback& on_started,
    download::InProgressDownloadManager::StartDownloadItemCallback callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);

  if (!info->is_new_download) {
    download::DownloadItemImpl* download = downloads_by_guid_[info->guid];
    if (!download || download->GetState() == download::DownloadItem::CANCELLED)
      download = nullptr;
    std::move(callback).Run(std::move(info), download,
                            should_persist_new_download_);
    OnDownloadStarted(download, on_started);
  } else {
    GetNextId(base::BindOnce(&DownloadManagerImpl::CreateNewDownloadItemToStart,
                             weak_factory_.GetWeakPtr(), std::move(info),
                             on_started, std::move(callback)));
  }
}
