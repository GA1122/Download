    ClearOriginsOnIOThread(
        storage::QuotaManager* quota_manager,
        const scoped_refptr<storage::SpecialStoragePolicy>&
            special_storage_policy,
        const StoragePartition::OriginMatcherFunction& origin_matcher,
        bool perform_storage_cleanup,
        base::OnceClosure callback,
        const std::set<url::Origin>& origins,
        blink::mojom::StorageType quota_storage_type) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  if (origins.empty()) {
    std::move(callback).Run();
    return;
  }

  base::RepeatingClosure done_callback = base::AdaptCallbackForRepeating(
      perform_storage_cleanup
          ? base::BindOnce(&PerformQuotaManagerStorageCleanup,
                           base::WrapRefCounted(quota_manager),
                           quota_storage_type, remove_mask_,
                           std::move(callback))
          : std::move(callback));

  size_t* deletion_task_count = new size_t(0u);
  (*deletion_task_count)++;
  for (const auto& origin : origins) {
    if (storage_origin_.has_value() && origin != *storage_origin_)
      continue;

    if (!origin_matcher.is_null() &&
        !origin_matcher.Run(origin.GetURL(), special_storage_policy.get())) {
      continue;
    }

    (*deletion_task_count)++;
    quota_manager->DeleteOriginData(
        origin, quota_storage_type,
        StoragePartitionImpl::GenerateQuotaClientMask(remove_mask_),
        base::BindOnce(&OnQuotaManagedOriginDeleted, origin, quota_storage_type,
                       deletion_task_count, done_callback));
  }
  (*deletion_task_count)--;

  CheckQuotaManagedDataDeletionStatus(deletion_task_count, done_callback);
}
