void DownloadManagerImpl::CheckForHistoryFilesRemoval() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  for (const auto& it : downloads_) {
    download::DownloadItemImpl* item = it.second.get();
    CheckForFileRemoval(item);
  }
}
