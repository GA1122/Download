bool AppCacheUpdateJob::MaybeLoadFromNewestCache(const GURL& url,
                                                 AppCacheEntry& entry) {
  if (update_type_ != UPGRADE_ATTEMPT)
    return false;

  AppCache* newest = group_->newest_complete_cache();
  AppCacheEntry* copy_me = newest->GetEntry(url);
  if (!copy_me || !copy_me->has_response_id())
    return false;

  loading_responses_.insert(
      LoadingResponses::value_type(copy_me->response_id(), url));
  storage_->LoadResponseInfo(manifest_url_, group_->group_id(),
                             copy_me->response_id(),
                             this);
  return true;
}
