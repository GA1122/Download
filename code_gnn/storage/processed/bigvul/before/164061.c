DownloadManagerImpl::RetrieveInProgressDownload(uint32_t id) {
  if (id == download::DownloadItem::kInvalidId)
    return nullptr;

  for (auto it = in_progress_downloads_.begin();
       it != in_progress_downloads_.end(); ++it) {
    if ((*it)->GetId() == id) {
      auto download = std::move(*it);
      in_progress_downloads_.erase(it);
      return download;
    }
  }

  return nullptr;
}
