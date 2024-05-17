void DownloadManagerImpl::CheckForFileRemoval(DownloadItemImpl* download_item) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  if ((download_item->GetState() == DownloadItem::COMPLETE) &&
      !download_item->GetFileExternallyRemoved() &&
      delegate_) {
    delegate_->CheckForFileExistence(
        download_item,
        base::BindOnce(&DownloadManagerImpl::OnFileExistenceChecked,
                       weak_factory_.GetWeakPtr(), download_item->GetId()));
  }
}
