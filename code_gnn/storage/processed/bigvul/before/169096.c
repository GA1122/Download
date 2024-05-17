void ReportStorageHistogramsAfterSave(
    const ArchiveManager::StorageStats& storage_stats) {
  const int kMB = 1024 * 1024;
  int free_disk_space_mb =
      static_cast<int>(storage_stats.free_disk_space / kMB);
  UMA_HISTOGRAM_CUSTOM_COUNTS("OfflinePages.SavePage.FreeSpaceMB",
                              free_disk_space_mb, 1, 500000, 50);

  int total_page_size_mb =
      static_cast<int>(storage_stats.total_archives_size() / kMB);
  UMA_HISTOGRAM_COUNTS_10000("OfflinePages.TotalPageSize", total_page_size_mb);
}
