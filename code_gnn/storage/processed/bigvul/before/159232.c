void DownloadManagerImpl::CheckForHistoryFilesRemoval() {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  for (const auto& it : downloads_) {
    DownloadItemImpl* item = it.second.get();
    CheckForFileRemoval(item);
  }
}
