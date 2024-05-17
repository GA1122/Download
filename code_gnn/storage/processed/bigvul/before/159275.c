 void DownloadManagerImpl::ShowDownloadInShell(DownloadItemImpl* download) {
   if (delegate_)
     delegate_->ShowDownloadInShell(download);
}
