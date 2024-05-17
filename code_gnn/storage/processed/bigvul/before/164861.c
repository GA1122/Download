void SetHiddenDownloadCallback(DownloadItem* item,
                               download::DownloadInterruptReason reason) {
  DownloadItemModel(item).SetShouldShowInShelf(false);
}
