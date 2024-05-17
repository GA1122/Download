bool DownloadManagerImpl::ShouldOpenDownload(
    download::DownloadItemImpl* item,
    const ShouldOpenDownloadCallback& callback) {
  if (!delegate_)
    return true;

