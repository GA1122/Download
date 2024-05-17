void AppCacheUpdateJob::OnGroupMadeObsolete(AppCacheGroup* group,
                                            bool success,
                                            int response_code) {
  DCHECK(master_entry_fetches_.empty());
  CancelAllMasterEntryFetches(AppCacheErrorDetails(
      "The cache has been made obsolete, "
      "the manifest file returned 404 or 410",
      APPCACHE_MANIFEST_ERROR,
      GURL(),
      response_code,
      false  ));
  if (success) {
    DCHECK(group->is_obsolete());
    NotifyAllAssociatedHosts(APPCACHE_OBSOLETE_EVENT);
    internal_state_ = COMPLETED;
    MaybeCompleteUpdate();
  } else {
    HandleCacheFailure(AppCacheErrorDetails(
                           "Failed to mark the cache as obsolete",
                           APPCACHE_UNKNOWN_ERROR, GURL(),  0,
                           false  ),
                       DB_ERROR,
                       GURL());
  }
}
