void DownloadManagerImpl::ShowDownloadInShell(
    download::DownloadItemImpl* download) {
  if (delegate_)
    delegate_->ShowDownloadInShell(download);
}
