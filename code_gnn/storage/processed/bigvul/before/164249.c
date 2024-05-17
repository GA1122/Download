void AppCacheUpdateJob::HandleCacheFailure(
    const blink::mojom::AppCacheErrorDetails& error_details,
    ResultType result,
    const GURL& failed_resource_url) {
  DCHECK(internal_state_ != CACHE_FAILURE);
  DCHECK(!error_details.message.empty());
  DCHECK(result != UPDATE_OK);
  internal_state_ = CACHE_FAILURE;
  LogHistogramStats(result, failed_resource_url);
  CancelAllUrlFetches();
  CancelAllMasterEntryFetches(error_details);
  NotifyAllError(error_details);
  DiscardInprogressCache();
  internal_state_ = COMPLETED;

  if (update_type_ == CACHE_ATTEMPT ||
      !IsEvictableError(result, error_details) ||
      service_->storage() != storage_) {
    DeleteSoon();
    return;
  }

  if (group_->first_evictable_error_time().is_null()) {
    group_->set_first_evictable_error_time(base::Time::Now());
    storage_->StoreEvictionTimes(group_);
    DeleteSoon();
    return;
  }

  base::TimeDelta kMaxEvictableErrorDuration = base::TimeDelta::FromDays(14);
  base::TimeDelta error_duration =
      base::Time::Now() - group_->first_evictable_error_time();
  if (error_duration > kMaxEvictableErrorDuration) {
    group_->SetUpdateAppCacheStatus(AppCacheGroup::IDLE);
    group_ = nullptr;
    service_->DeleteAppCacheGroup(manifest_url_,
                                  base::BindOnce(EmptyCompletionCallback));
  }

  DeleteSoon();   
}
