void OfflinePageModelTaskified::OnClearCachedPagesDone(
    base::Time start_time,
    size_t deleted_page_count,
    ClearStorageResult result) {
  last_clear_cached_pages_time_ = start_time;
}
