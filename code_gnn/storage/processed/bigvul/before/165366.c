void StoragePartitionImpl::DataDeletionHelper::ClearDataOnUIThread(
    const GURL& storage_origin,
    const OriginMatcherFunction& origin_matcher,
    CookieDeletionFilterPtr cookie_deletion_filter,
    const base::FilePath& path,
    DOMStorageContextWrapper* dom_storage_context,
    storage::QuotaManager* quota_manager,
    storage::SpecialStoragePolicy* special_storage_policy,
    storage::FileSystemContext* filesystem_context,
    network::mojom::CookieManager* cookie_manager,
    bool perform_storage_cleanup,
    const base::Time begin,
    const base::Time end) {
  DCHECK_NE(remove_mask_, 0u);
  DCHECK(!callback_.is_null());

  IncrementTaskCountOnUI();
  base::RepeatingClosure decrement_callback = base::BindRepeating(
      &DataDeletionHelper::DecrementTaskCount, base::Unretained(this));

  if (remove_mask_ & REMOVE_DATA_MASK_COOKIES) {
    DCHECK(!cookie_deletion_filter->created_after_time.has_value());
    DCHECK(!cookie_deletion_filter->created_before_time.has_value());

    if (!begin.is_null())
      cookie_deletion_filter->created_after_time = begin;
    if (!end.is_null())
      cookie_deletion_filter->created_before_time = end;

    cookie_manager->DeleteCookies(
        std::move(cookie_deletion_filter),
        base::BindOnce(
            &OnClearedCookies,
            base::BindOnce(&OwnsReference::Callback,
                           std::make_unique<OwnsReference>(this))));
  }

  if (remove_mask_ & REMOVE_DATA_MASK_INDEXEDDB ||
      remove_mask_ & REMOVE_DATA_MASK_WEBSQL ||
      remove_mask_ & REMOVE_DATA_MASK_APPCACHE ||
      remove_mask_ & REMOVE_DATA_MASK_FILE_SYSTEMS ||
      remove_mask_ & REMOVE_DATA_MASK_SERVICE_WORKERS ||
      remove_mask_ & REMOVE_DATA_MASK_CACHE_STORAGE) {
    IncrementTaskCountOnUI();
    base::PostTaskWithTraits(
        FROM_HERE, {BrowserThread::IO},
        base::BindOnce(
            &DataDeletionHelper::ClearQuotaManagedDataOnIOThread,
            base::Unretained(this), base::WrapRefCounted(quota_manager), begin,
            storage_origin, base::WrapRefCounted(special_storage_policy),
            origin_matcher, perform_storage_cleanup, decrement_callback));
  }

  if (remove_mask_ & REMOVE_DATA_MASK_LOCAL_STORAGE) {
    IncrementTaskCountOnUI();
    ClearLocalStorageOnUIThread(base::WrapRefCounted(dom_storage_context),
                                base::WrapRefCounted(special_storage_policy),
                                origin_matcher, storage_origin,
                                perform_storage_cleanup, begin, end,
                                decrement_callback);

    if (storage_origin.is_empty()) {
      IncrementTaskCountOnUI();
      ClearSessionStorageOnUIThread(
          base::WrapRefCounted(dom_storage_context),
          base::WrapRefCounted(special_storage_policy), origin_matcher,
          perform_storage_cleanup, decrement_callback);
    }
  }

  if (remove_mask_ & REMOVE_DATA_MASK_SHADER_CACHE) {
    IncrementTaskCountOnUI();
    base::PostTaskWithTraits(FROM_HERE, {BrowserThread::IO},
                             base::BindOnce(&ClearShaderCacheOnIOThread, path,
                                            begin, end, decrement_callback));
  }

#if BUILDFLAG(ENABLE_PLUGINS)
  if (remove_mask_ & REMOVE_DATA_MASK_PLUGIN_PRIVATE_DATA) {
    IncrementTaskCountOnUI();
    filesystem_context->default_file_task_runner()->PostTask(
        FROM_HERE, base::BindOnce(&ClearPluginPrivateDataOnFileTaskRunner,
                                  base::WrapRefCounted(filesystem_context),
                                  storage_origin, origin_matcher,
                                  base::WrapRefCounted(special_storage_policy),
                                  begin, end, std::move(decrement_callback)));
  }
#endif   

  DecrementTaskCount();
}
