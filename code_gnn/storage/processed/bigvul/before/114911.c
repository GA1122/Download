DownloadItem* GetDownloadItemFromId(int id, DownloadManager* download_manager) {
  std::vector<DownloadItem*> downloads;
  download_manager->GetAllDownloads(FilePath(), &downloads);
  DownloadItem* selected_item = NULL;

  for (std::vector<DownloadItem*>::iterator it = downloads.begin();
       it != downloads.end();
       it++) {
    DownloadItem* curr_item = *it;
    if (curr_item->GetId() == id) {
      selected_item = curr_item;
      break;
    }
  }
  return selected_item;
}
