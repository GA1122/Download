void StoragePartitionImpl::ClearDataImpl(
    uint32_t remove_mask,
    uint32_t quota_storage_remove_mask,
    const GURL& storage_origin,
    const OriginMatcherFunction& origin_matcher,
    CookieDeletionFilterPtr cookie_deletion_filter,
    bool perform_storage_cleanup,
    const base::Time begin,
    const base::Time end,
    base::OnceClosure callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  DataDeletionHelper* helper = new DataDeletionHelper(
      remove_mask, quota_storage_remove_mask,
      base::BindOnce(&StoragePartitionImpl::DeletionHelperDone,
                     weak_factory_.GetWeakPtr(), std::move(callback)));
  deletion_helpers_running_++;
  helper->ClearDataOnUIThread(
      storage_origin, origin_matcher, std::move(cookie_deletion_filter),
      GetPath(), dom_storage_context_.get(), quota_manager_.get(),
      special_storage_policy_.get(), filesystem_context_.get(),
      GetCookieManagerForBrowserProcess(), perform_storage_cleanup, begin, end);
}
