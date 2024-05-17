void OfflinePageModelTaskified::PostClearCachedPagesTask(bool is_initializing) {
  if (is_initializing) {
    base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
        FROM_HERE,
        base::Bind(&OfflinePageModelTaskified::PostClearCachedPagesTask,
                   weak_ptr_factory_.GetWeakPtr(), false),
        kInitializingTaskDelay);
  }

  if (GetCurrentTime() - last_clear_cached_pages_time_ <
      kClearStorageInterval) {
    return;
  }

  ClearCachedPages();
}
