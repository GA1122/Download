void DownloadManagerImpl::GetAllDownloads(
    download::SimpleDownloadManager::DownloadVector* downloads) {
  for (const auto& it : downloads_) {
    downloads->push_back(it.second.get());
  }
}