void ChromeDownloadManagerDelegate::RemoveItemsFromPersistentStoreBetween(
    base::Time remove_begin,
    base::Time remove_end) {
  download_history_->RemoveEntriesBetween(remove_begin, remove_end);
}
