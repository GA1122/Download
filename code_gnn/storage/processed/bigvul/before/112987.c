DownloadFile* DownloadFileManager::GetDownloadFile(
    DownloadId global_id) {
  DownloadFileMap::iterator it = downloads_.find(global_id);
  return it == downloads_.end() ? NULL : it->second;
}
