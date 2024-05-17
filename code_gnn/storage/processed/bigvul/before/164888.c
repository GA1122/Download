DownloadManagerImpl::RetrieveInProgressDownload(uint32_t id) {
  if (base::ContainsKey(in_progress_downloads_, id)) {
    auto download = std::move(in_progress_downloads_[id]);
    in_progress_downloads_.erase(id);
    return download;
  }
  return nullptr;
}
