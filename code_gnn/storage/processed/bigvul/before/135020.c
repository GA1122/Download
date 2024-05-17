void AppCacheUpdateJob::OnGroupAndNewestCacheStored(AppCacheGroup* group,
                                                    AppCache* newest_cache,
                                                    bool success,
                                                    bool would_exceed_quota) {
  DCHECK(stored_state_ == STORING);
  if (success) {
    stored_state_ = STORED;
    MaybeCompleteUpdate();   
    return;
  }

  stored_state_ = UNSTORED;

  if (newest_cache != group->newest_complete_cache())
    inprogress_cache_ = newest_cache;

  ResultType result = DB_ERROR;
  AppCacheErrorReason reason = APPCACHE_UNKNOWN_ERROR;
  std::string message("Failed to commit new cache to storage");
  if (would_exceed_quota) {
    message.append(", would exceed quota");
    result = QUOTA_ERROR;
    reason = APPCACHE_QUOTA_ERROR;
  }
  HandleCacheFailure(
      AppCacheErrorDetails(message, reason, GURL(), 0,
          false  ),
      result,
      GURL());
}
