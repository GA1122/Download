void ChromeDownloadManagerDelegate::AddItemToPersistentStore(
    DownloadItem* item) {
  download_history_->AddEntry(item,
      base::Bind(&ChromeDownloadManagerDelegate::OnItemAddedToPersistentStore,
                 base::Unretained(this)));
}
