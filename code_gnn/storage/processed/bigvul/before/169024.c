void OfflinePageModelTaskified::PostClearLegacyTemporaryPagesTask() {
  base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
      FROM_HERE,
      base::Bind(&OfflinePageModelTaskified::ClearLegacyTemporaryPages,
                 weak_ptr_factory_.GetWeakPtr()),
      kInitializingTaskDelay);
}
