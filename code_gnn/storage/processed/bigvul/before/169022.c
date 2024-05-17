void OfflinePageModelTaskified::PostCheckMetadataConsistencyTask(
    bool is_initializing) {
  if (is_initializing) {
    base::ThreadTaskRunnerHandle::Get()->PostDelayedTask(
        FROM_HERE,
        base::Bind(&OfflinePageModelTaskified::PostCheckMetadataConsistencyTask,
                   weak_ptr_factory_.GetWeakPtr(), false),
        kInitializingTaskDelay);
    return;
  }

  CheckTemporaryPagesConsistency();
  CheckPersistentPagesConsistency();
}
